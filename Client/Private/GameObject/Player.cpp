#include "GameObject/Player.h"

#include "Component/EffectComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"
#include "Component/NavigationComponent.h"
#include "Component/CloudStationComp.h"

#include "GameObject/GameObjectFactory.h"
#include "GameObject/UI_Lockon.h"
#include "GameObject/Weapon_Buster.h"
#include "GameObject/UI_Player.h"
#include "GameObject/DamageCollision.h"
#include "GameObject/Door_Common.h"
#include "GameObject/Weapon_LaserEmission.h"
#include "GameObject/Weapon_SpreadBuster.h"
#include "GameObject/Weapon_HomingMissile.h"
#include "GameObject/Weapon_Machinegun.h"
#include "GameObject/Weapon_HyperShell.h"
#include "GameObject/Weapon_Blade.h"
#include "GameObject/ReaverBot_Fingerii.h"
#include "GameObject/ItemChest.h"
#include "GameObject/Item_Deflector.h"
#include "GameObject/DynamicCamera.h"
#include "GameObject/Weapon_BusterCannon.h"

#include "CloudStation/CloudStation_Player.h"

#include "System/RenderMgr.h"


CPlayer::CPlayer()
{
    Set_Name(L"Player");
}

CPlayer::CPlayer(const CPlayer& rhs)
    : Base(rhs)
{
    
}

HRESULT CPlayer::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    
    FAILED_CHECK_RETURN(Add_Component(L"CameraPivot", m_pCameraPivotComp = CPivotComponent::Create()), E_FAIL);
    m_pCameraPivotComp->OffsetTransform().Set_Position(0.f, 1.3f, 0.f);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/RockVolnutt.amodel");
    m_pModelComp->Set_Animation(0, 1.f, true);

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Transform().Set_Position(0.f, 0.8f, 0.f);
    m_pColliderComp->Transform().Set_Scale(1.f, 0.8f, 1.f);
    m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
                                        | COLLAYER_ITEM | COLLAYER_OBJECT | COLLAYER_ATTACKER);

    TeamAgentComp().Set_TeamID(ETEAM_PLAYER);

    /*m_pModelComp->Create_Mask(L"Main", L"Armature", true);
    m_pModelComp->Create_Mask(L"Head", L"Armature", false);
    m_pModelComp->Create_Mask(L"Leg", L"Armature", false);
    m_pModelComp->Create_Mask(L"LeftArm", L"Armature", false);
    m_pModelComp->Create_Mask(L"RightArm", L"Armature", false);

    m_pModelComp->Active_BoneMask(2, L"bone_000");*/

    CNavigationComponent::TCloneDesc tDesc = { 0 };
    FAILED_CHECK_RETURN(Add_Component(L"Navigation",
        m_pNaviComp = Cast<CNavigationComponent*>(GI()->Clone_PrototypeComp(TEXT("Prototype_Component_Navigation"), VPCast(&tDesc)))), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"CloudStation", m_pCloudStationComp = CCloudStationComp::Create()), E_FAIL);
    m_pCloudStationComp->Open_CloudStation_Session(TEXT("Player"), CCloudStation_Player::Create());
    m_pCloudStationComp->Connect_CloudStation(TEXT("Player"));

    m_vAcceleration = m_vMoveSpeed = m_vMaxMoveSpeed = { 6.f, 10.f, 6.f };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    return S_OK;
}

HRESULT CPlayer::Initialize_Prototype(FSerialData& InputData)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(InputData), E_FAIL);

    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData ProtoData;
        InputData.Get_ObjectFromArray("Components", i, ProtoData);

        _uint iComponentID = 0;
        if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strName = "";
        if (FAILED(ProtoData.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::CommonModel):
            NULL_CHECK_RETURN(m_pModelComp = CCommonModelComp::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pModelComp)))
                return E_FAIL;
            m_pModelComp->Set_Animation(0, 1.f, true);
            break;
        case ECast(EComponentID::Collider):
            NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pColliderComp)))
                return E_FAIL;
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            //m_pColliderComp->EnterToPhysics(0);
            break;
        case ECast(EComponentID::CloudStation):
            NULL_CHECK_RETURN(m_pCloudStationComp = CCloudStationComp::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pCloudStationComp)))
                return E_FAIL;
            m_pCloudStationComp->Open_CloudStation_Session(TEXT("Player"), CCloudStation_Player::Create());
            m_pCloudStationComp->Connect_CloudStation(TEXT("Player"));
            break;
        case ECast(EComponentID::Pivot):
            NULL_CHECK_RETURN(m_pCameraPivotComp = CPivotComponent::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pCameraPivotComp)))
                return E_FAIL;
            break;
        }
    }

    TeamAgentComp().Set_TeamID(ETEAM_PLAYER);

    _float2 vSpeed = {};
    InputData.Get_Data("MoveSpeed", vSpeed.x);
    InputData.Get_Data("JumpSpeed", vSpeed.y);
    m_vMoveSpeed = m_vMaxMoveSpeed = { vSpeed.x, vSpeed.y, vSpeed.x };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    return S_OK;
}

HRESULT CPlayer::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CPlayer::Initialize(FSerialData& InputData)
{
    FAILED_CHECK_RETURN(__super::Initialize(InputData), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(InputData), E_FAIL);

    _float2 vSpeed = {};
    InputData.Get_Data("MoveSpeed", vSpeed.x);
    InputData.Get_Data("JumpSpeed", vSpeed.y);
    m_vMoveSpeed = m_vMaxMoveSpeed = { vSpeed.x, vSpeed.y, vSpeed.x };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CPlayer::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_ActionKey.Reset();
    if (m_fInvisibleTime.Increase(fTimeDelta))
        m_bInvisible = false;

    m_fKnockDownDelay.Increase(fTimeDelta);
    // 넉다운 수치 시간당 감소
    m_fKnockDownValue.Increase(-fTimeDelta);

    Lockon_Active(fTimeDelta);

    Input_Weapon(fTimeDelta);
    m_State_Act.Get_StateFunc()(this, fTimeDelta);
    Move_Update(fTimeDelta);

    GrabingUnit();
    
    m_pColliderComp->Tick(fTimeDelta);
    m_pCameraPivotComp->Tick(fTimeDelta);

    Update_ToCloudStation();
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    if (IsState(EGObjectState::Drawing))
        m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
    SUPER::Render();
    
    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

HRESULT CPlayer::Render_Shadow()
{
    auto pEffect = m_pModelComp->EffectComp();
    _float4x4 TempFloat4x4 = m_pModelComp->Calculate_TransformFloat4x4FromParent();

    if (FAILED(pEffect->Bind_Matrix("g_WorldMatrix", &TempFloat4x4)))
        return E_FAIL;

    _float4x4		ViewMatrix, ProjMatrix;

    XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(40.f, 30.f, 40.f, 1.f), XMVectorSet(40.f, 0.f, 40.f, 1.f), XMVectorSet(0.f, 0.f, 1.f, 0.f)));
    XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), g_iWindowSizeX / (float)g_iWindowSizeY, 0.1f, 300.f));

    if (FAILED(pEffect->Bind_Matrix("g_ViewMatrix", &ViewMatrix)))
        return E_FAIL;
    if (FAILED(pEffect->Bind_Matrix("g_ProjMatrix", &ProjMatrix)))
        return E_FAIL;

    m_pModelComp->BindAndRender_Meshes(2, true);

    return S_OK;
}

void CPlayer::OnCreated()
{
    TurnOn_State(EGObjectState::Shadow);
    Register_State();
}

void CPlayer::OnDeleted()
{
    // 상호참조 문제로 여기서 해제
    Safe_Release(m_pDynamicCamera);
}

void CPlayer::BeginPlay()
{
    m_pDynamicCamera = DynCast<CDynamicCamera*>(m_pGI->Find_GameObjectByName(TEXT("DynamicCamera")));
    Safe_AddRef(m_pDynamicCamera);

    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
        | COLLAYER_ITEM | COLLAYER_OBJECT | COLLAYER_ATTACKER);
    if (m_pColliderComp)
        m_pColliderComp->EnterToPhysics(0);

    m_pCloudStationComp->Open_CloudStation_Session(TEXT("Player"), CCloudStation_Player::Create());
    m_pCloudStationComp->Connect_CloudStation(TEXT("Player"));

    if (nullptr == m_pPlayerCloud && nullptr != m_pCloudStationComp)
    {
        if (SUCCEEDED(m_pCloudStationComp->Connect_CloudStation(TEXT("Player"))))
        {
            m_pPlayerCloud = m_pCloudStationComp->Get_LastCloudStation<CCloudStation_Player>();
            Safe_AddRef(m_pPlayerCloud);
        }
    }

    if (m_pPlayerCloud)
    {
        m_pPlayerCloud->Access_HP(CCloudStation::EMode::Download, m_fHP);
        m_pPlayerCloud->Access_Money(CCloudStation::EMode::Download, m_iMoney);

        _float3 vPos = {}, vLook = {};
        m_pPlayerCloud->Access_StartPos(CCloudStation::EMode::Download, vPos);
        m_pPlayerCloud->Access_StartLook(CCloudStation::EMode::Download, vLook);

        Transform().Set_Position(vPos);
        Transform().Look_At_OnLand(XMLoadFloat3(&vPos) + XMLoadFloat3(&vLook));
    }
}

void CPlayer::EndPlay()
{
    
}

CPlayer* CPlayer::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CPlayer* CPlayer::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CPlayer::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CPlayer::Free()
{
    SUPER::Free();

    Safe_Release(m_pLockon_Target);
    Safe_Release(m_pLockon_UI);
    Safe_Release(m_pPlayerCloud);
    Safe_Release(m_pLaserEmission);
    Safe_Release(m_pBlade);
    Safe_Release(m_pGrabUnit);
}

FSerialData CPlayer::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);
    Data.Add_Member("MoveSpeed", m_vMaxMoveSpeed.x);
    Data.Add_Member("JumpSpeed", m_vMaxMoveSpeed.y);

    return Data;
}

FSerialData CPlayer::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);
    Data.Add_Member("MoveSpeed", m_vMaxMoveSpeed.x);
    Data.Add_Member("JumpSpeed", m_vMaxMoveSpeed.y);

    return Data;
}

HRESULT CPlayer::Initialize_Component()
{
    GI()->Add_GameObject(CUI_Player::Create());

    return S_OK;
}

HRESULT CPlayer::Initialize_Component(FSerialData& InputData)
{
    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData InputProto;
        InputData.Get_ObjectFromArray("Components", i, InputProto);

        _uint iComponentID = 0;
        if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strProtoName = "";
        if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
            return E_FAIL;

        string strName = "";
        if (FAILED(InputProto.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::CommonModel):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pModelComp = DynCast<CCommonModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pModelComp->Set_Animation(0, 1.f, true);
            m_pModelComp->Deactive_Mesh(11);
            m_pModelComp->Deactive_Mesh(15);
            m_pModelComp->Deactive_Mesh(16);
            m_pModelComp->Deactive_Mesh(17);
            m_pModelComp->Deactive_Mesh(18);
            m_pModelComp->Deactive_Mesh(19);
            m_pModelComp->Deactive_Mesh(20);
            m_pModelComp->Deactive_Mesh(21);
            m_pModelComp->Deactive_Mesh(22);
            m_pModelComp->Deactive_Mesh(23);
            break;
        case ECast(EComponentID::Collider):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pColliderComp = DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            m_pColliderComp->Set_CollisionKinematic();
            break;
        case ECast(EComponentID::CloudStation):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pCloudStationComp = DynCast<CCloudStationComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            break;
        case ECast(EComponentID::Pivot):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pCameraPivotComp = DynCast<CPivotComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            break;
        }
    }

    CNavigationComponent::TCloneDesc tDesc = { 0 };
    FAILED_CHECK_RETURN(Add_Component(L"Navigation",
        m_pNaviComp = Cast<CNavigationComponent*>(GI()->Clone_PrototypeComp(TEXT("Prototype_Component_Navigation"), VPCast(&tDesc)))), E_FAIL);

    GI()->Add_GameObject(CUI_Player::Create());

    TeamAgentComp().Set_TeamID(ETEAM_PLAYER);

    return S_OK;
}

void CPlayer::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

    CItemChest* pChest = DynCast<CItemChest*>(pDst);
    if (nullptr != pChest)
    {
        _float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
        _vector vSimNormal = {};
        vSimNormal = XMLoadFloat3(&vNormal);
        Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
        if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
            m_bIsOnGround = true;

        if (m_ActionKey.IsAct(EActionKey::Interaction))
        {
            _vector vLook = pDst->Transform().Get_LookNormalizedVector();
            _vector vPosLook = XMVector3Normalize(Transform().Get_PositionVector() - pDst->Transform().Get_PositionVector());
            _float fDot = XMVectorGetX(XMVector3Dot(vPosLook, Transform().Get_LookNormalizedVector()));
            _float fDotLookAndPos = XMVectorGetX(XMVector3Dot(vPosLook, vLook));
            if (fDot <= -0.8f && fDotLookAndPos >= 0.8f)
            {
                pChest->Open_Chest(MakeDelegate(this, &ThisClass::Gotcha), MakeDelegate(this, &ThisClass::EndOpenChest));
                m_State_Act.Set_State(EState_Act::ItemGetting);
            }
        }
    }

    CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
    if (pEnemy)
    {
        if (CTeamAgentComp::ERelation::Hostile ==
            CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
        {
            _bool bIsGrab = { false };
            if (m_pGrabUnit == nullptr)
            {
                if (m_ActionKey.IsAct(EActionKey::Grab))
                {
                    if (pEnemy->Get_IsCanGrab())
                    {
                        m_State_Act.Set_State(EState_Act::Grab);
                        DeleteLaser();
                        DeleteBlade();
                        m_pGrabUnit = pEnemy;
                        Safe_AddRef(pEnemy);
                        pEnemy->Set_Grabbed(true);
                        bIsGrab = true;
                    }
                }
            }
            else
            {
                if (m_pGrabUnit == pDst)
                    bIsGrab = true;
            }

            if (!bIsGrab)
            {
                if (m_fKnockDownDelay.IsMax() && !m_bInvisible)
                {
                    m_fKnockDownDelay.Reset();
                    if (m_fKnockDownValue.Increase(2.5f))
                    {
                        m_State_Act.Set_State(EState_Act::DamagedHeavy);
                        DeleteLaser();
                        DeleteBlade();
                        ThrowUnit();
                        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
                    }
                    else
                    {
                        m_State_Act.Set_State(EState_Act::DamagedLight);
                        DeleteLaser();
                        DeleteBlade();
                        ThrowUnit();
                        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
                    }
                    m_fHP.Increase(-5.f);
                }
            }
        }
    }

    CDamageCollision* pDamageCollision = DynCast<CDamageCollision*>(pDst);
    if (pDamageCollision)
    {
        if (CTeamAgentComp::ERelation::Hostile ==
            CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pDamageCollision->TeamAgentComp()))
        {
            if (!m_bInvisible)
            {
                m_State_Act.Set_State(EState_Act::DamagedHeavy);
                DeleteLaser();
                DeleteBlade();
                ThrowUnit();
                GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
                m_fHP.Increase(-5.f);
            }
        }
    }

    CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
    if (pDoor)
    {
        if (m_ActionKey.IsAct(EActionKey::Interaction))
            pDoor->OpenDoor();

        _float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
        _vector vSimNormal = {};
        vSimNormal = XMLoadFloat3(&vNormal);
        Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
        if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
            m_bIsOnGround = true;


    }

    CItem_Deflector* pDeflector = DynCast<CItem_Deflector*>(pDst);
    if (pDeflector)
    {
        m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("get_reflector_shard.mp3"), CHANNELID::SOUND_VFX2, 1.f);
        pDeflector->Obtain_Money(m_iMoney);
        pDeflector->Set_Dead();
    }
}

void CPlayer::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CPlayer::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}

void CPlayer::Update_ToCloudStation()
{
    if (nullptr != m_pPlayerCloud)
    {
        m_pPlayerCloud->Set_HP(m_fHP);
    }
}

void CPlayer::Register_State()
{
    for (_uint i = 0; i < ECast(EActionKey::Size); i++)
        m_ActionKey.Add_Action(Cast<EActionKey>(i));

    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Run, &ThisClass::ActState_Run);
    m_State_Act.Add_Func(EState_Act::Walk, &ThisClass::ActState_Walk);
    m_State_Act.Add_Func(EState_Act::Ready_Jump, &ThisClass::ActState_Ready_Jump);
    m_State_Act.Add_Func(EState_Act::Jump_Up, &ThisClass::ActState_Jump_Up);
    m_State_Act.Add_Func(EState_Act::Jump_Down, &ThisClass::ActState_Jump_Down);
    m_State_Act.Add_Func(EState_Act::Landing, &ThisClass::ActState_Landing);
    m_State_Act.Add_Func(EState_Act::Buster, &ThisClass::ActState_Buster);
    m_State_Act.Add_Func(EState_Act::DamagedLight, &ThisClass::ActState_DamagedLight);
    m_State_Act.Add_Func(EState_Act::DamagedHeavy, &ThisClass::ActState_DamagedHeavy);
    m_State_Act.Add_Func(EState_Act::KnockDown, &ThisClass::ActState_KnockDown);
    m_State_Act.Add_Func(EState_Act::StandUp, &ThisClass::ActState_StandUp);
    m_State_Act.Add_Func(EState_Act::ReadyLaser, &ThisClass::ActState_ReadyLaser);
    m_State_Act.Add_Func(EState_Act::ShootingLaser, &ThisClass::ActState_ShootingLaser);
    m_State_Act.Add_Func(EState_Act::EndLaser, &ThisClass::ActState_EndLaser);
    m_State_Act.Add_Func(EState_Act::Homing, &ThisClass::ActState_Homing);
    m_State_Act.Add_Func(EState_Act::SpreadBuster, &ThisClass::ActState_SpreadBuster);
    m_State_Act.Add_Func(EState_Act::ChargeShot, &ThisClass::ActState_ChargeShot);

    m_State_Act.Add_Func(EState_Act::ReadyBusterCannon, &ThisClass::ActState_ReadyBusterCannon);
    m_State_Act.Add_Func(EState_Act::ShootBusterCannon, &ThisClass::ActState_ShootBusterCannon);
    m_State_Act.Add_Func(EState_Act::EndBusterCannon, &ThisClass::ActState_EndBusterCannon);
    m_State_Act.Add_Func(EState_Act::ReadyHyperShell, &ThisClass::ActState_ReadyHyperShell);
    m_State_Act.Add_Func(EState_Act::ShootHyperShell, &ThisClass::ActState_ShootHyperShell);
    m_State_Act.Add_Func(EState_Act::EndHyperShell, &ThisClass::ActState_EndHyperShell);
    m_State_Act.Add_Func(EState_Act::ReadyMachinegun, &ThisClass::ActState_ReadyMachinegun);
    m_State_Act.Add_Func(EState_Act::ShootingMachinegun, &ThisClass::ActState_ShootingMachinegun);
    m_State_Act.Add_Func(EState_Act::EndMachinegun, &ThisClass::ActState_EndMachinegun);
    m_State_Act.Add_Func(EState_Act::BladeAttack1, &ThisClass::ActState_BladeAttack1);
    m_State_Act.Add_Func(EState_Act::BladeAttack2, &ThisClass::ActState_BladeAttack2);
    m_State_Act.Add_Func(EState_Act::BladeEnd, &ThisClass::ActState_BladeEnd);
    m_State_Act.Add_Func(EState_Act::Grab, &ThisClass::ActState_Grab);
    m_State_Act.Add_Func(EState_Act::Grabbing, &ThisClass::ActState_Grabbing);
    m_State_Act.Add_Func(EState_Act::Throw, &ThisClass::ActState_Throw);
    m_State_Act.Add_Func(EState_Act::Squat, &ThisClass::ActState_Squat);
    m_State_Act.Add_Func(EState_Act::ItemGetting, &ThisClass::ActState_ItemGetting);
    m_State_Act.Add_Func(EState_Act::ItemGet, &ThisClass::ActState_ItemGet);

    m_State_Act.Set_State(EState_Act::Idle);
}

void CPlayer::Move_Update(const _float& fTimeDelta)
{
    _float3 vfPos = Transform().Get_PositionFloat3();
    _vector vPos = Transform().Get_PositionVector();

    m_vVelocity.y -= m_vAcceleration.y * fTimeDelta;
    vfPos.y += m_vVelocity.y * fTimeDelta;
    if (vfPos.y <= 0.f)
    {
        vfPos.y = 0.f;
        m_bIsOnGround = true;
    }
    else
        m_bIsOnGround = false;

    Transform().Set_PositionY(vfPos.y);

    m_bIsMoving = false;
    m_vLookDirection = {};
    m_ePrevMoveDir = m_eMoveDir;
    if (m_ActionKey.IsAct(EActionKey::MoveForward))
    {
        m_vLookDirection.z = 1.f;
        if (m_ActionKey.IsAct(EActionKey::MoveSlow))
            m_vVelocity.z += m_vAcceleration.z * 0.3f * fTimeDelta;
        else if (m_ActionKey.IsAct(EActionKey::MoveFast))
            m_vVelocity.z += m_vAcceleration.z * 1.5f * fTimeDelta;
        else
            m_vVelocity.z += m_vAcceleration.z * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_FORWARD;
    }
    else if (m_ActionKey.IsAct(EActionKey::MoveBackward))
    {
        m_vLookDirection.z = -1.f;
        if (m_ActionKey.IsAct(EActionKey::MoveSlow))
            m_vVelocity.z -= m_vAcceleration.z * 0.3f * fTimeDelta;
        else if (m_ActionKey.IsAct(EActionKey::MoveFast))
            m_vVelocity.z -= m_vAcceleration.z * 1.5f * fTimeDelta;
        else
            m_vVelocity.z -= m_vAcceleration.z * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_BACK;
    }
    else
    {
        if (m_bIsOnGround)
        {
            if (!m_ActionKey.IsAct(EActionKey::LowFrict))
                m_vVelocity.z *= 0.5f;
            else
                m_vVelocity.z *= 0.95f;
        }
        else
            m_vVelocity.z *= 0.998f;
    }

    if (m_ActionKey.IsAct(EActionKey::MoveRight))
    {
        m_vLookDirection.x = 1.f;
        if (m_ActionKey.IsAct(EActionKey::MoveSlow))
            m_vVelocity.x += m_vAcceleration.x * 0.3f * fTimeDelta;
        else if (m_ActionKey.IsAct(EActionKey::MoveFast))
            m_vVelocity.x += m_vAcceleration.x * 1.5f * fTimeDelta;
        else
            m_vVelocity.x += m_vAcceleration.x * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_RIGHT;
    }
    else if (m_ActionKey.IsAct(EActionKey::MoveLeft))
    {
        m_vLookDirection.x = -1.f;
        if (m_ActionKey.IsAct(EActionKey::MoveSlow))
            m_vVelocity.x -= m_vAcceleration.x * 0.3f * fTimeDelta;
        else if (m_ActionKey.IsAct(EActionKey::MoveFast))
            m_vVelocity.x -= m_vAcceleration.x * 1.5f * fTimeDelta;
        else
            m_vVelocity.x -= m_vAcceleration.x * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_LEFT;
    }
    else
    {
        if (m_bIsOnGround)
        {
            if (!m_ActionKey.IsAct(EActionKey::LowFrict))
                m_vVelocity.x *= 0.5f;
            else
                m_vVelocity.x *= 0.95f;
        }
        else
            m_vVelocity.x *= 0.998f;
    }

    if (m_bIsOnGround)
    {
        if (m_ActionKey.IsAct(EActionKey::Jump))
        {
            if (!m_ActionKey.IsAct(EActionKey::JumpLow))
                m_vVelocity.y = m_vMoveSpeed.y;
            else
                m_vVelocity.y = m_vMoveSpeed.y * 0.65f;
            m_bIsOnGround = false;

        }
    }

    if (m_vVelocity.x > m_vMoveSpeed.x)
        m_vVelocity.x = m_vMoveSpeed.x;
    else if (m_vVelocity.x < -m_vMoveSpeed.x)
        m_vVelocity.x = -m_vMoveSpeed.x;

    if (m_vVelocity.z > m_vMoveSpeed.z)
        m_vVelocity.z = m_vMoveSpeed.z;
    else if (m_vVelocity.z < -m_vMoveSpeed.z)
        m_vVelocity.z = -m_vMoveSpeed.z;

    Transform().MoveForward(m_vVelocity.z * fTimeDelta);
    Transform().MoveRightward(m_vVelocity.x * fTimeDelta);
    if (!m_pNaviComp->IsMove(Transform().Get_PositionVector()))
        Transform().Set_Position(vPos);
}

void CPlayer::Look_Update(const _float& fTimeDelta)
{
    if (nullptr == m_pLockon_Target)
        Transform().TurnAxis(_float3(0.f, 1.f, 0.f), Cast<_float>(GI()->Get_DIMouseMove(DIMS_X)) * 0.1f * fTimeDelta);
}

void CPlayer::Input_Move(const _float& fTimeDelta)
{
    if (GI()->IsKey_Pressing(DIK_W))
        m_ActionKey.Act(EActionKey::MoveForward);
    else if (GI()->IsKey_Pressing(DIK_S))
        m_ActionKey.Act(EActionKey::MoveBackward);

    if (GI()->IsKey_Pressing(DIK_D))
        m_ActionKey.Act(EActionKey::MoveRight);
    else if (GI()->IsKey_Pressing(DIK_A))
        m_ActionKey.Act(EActionKey::MoveLeft);

    if (m_bIsOnGround && GI()->IsKey_Pressed(DIK_SPACE))
    {
        m_ActionKey.Act(EActionKey::Jump);
    }

    if (m_bIsOnGround && GI()->IsKey_Pressed(DIK_E))
    {
        m_ActionKey.Act(EActionKey::Interaction);
    }
}

void CPlayer::Input_Weapon(const _float& fTimeDelta)
{
    if (GI()->IsKey_Pressed(DIK_Z))
    {
        if (ECast(EMainWeapon::ChargeBuster) == ECast(m_eMainWeapon))
            ChangeMainWeapon(EMainWeapon::None);
        else
            ChangeMainWeapon(Cast<EMainWeapon>(ECast(m_eMainWeapon) + 1));
    }

    //if (m_bIsCanUseSubWeapons)
    {
        if (GI()->IsKey_Pressed(DIK_C))
        {
            if (ECast(ESubWeapon::SpreadBusterArm) == ECast(m_eSubWeapon))
                ChangeSubWeapon(ESubWeapon::ThrowArm);
            else
                ChangeSubWeapon(Cast<ESubWeapon>(ECast(m_eSubWeapon) + 1));
        }
    }

    switch (m_eMainWeapon)
    {
    case EMainWeapon::None:
        
        break;
    case EMainWeapon::Buster:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_LB))
            m_ActionKey.Act(EActionKey::Buster);
        break;
    case EMainWeapon::ChargeBuster:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_LB))
            m_ActionKey.Act(EActionKey::ChargeBuster);
        break;
    default:
        break;
    }

    switch (m_eSubWeapon)
    {
    case ESubWeapon::ThrowArm:
        if (nullptr == m_pGrabUnit)
        {
            if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
                m_ActionKey.Act(EActionKey::Grab);
        }
        else
        {
            m_ActionKey.Act(EActionKey::Grab);
            if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
                m_ActionKey.Act(EActionKey::Throw);
        }
        break;
    case ESubWeapon::LaserArm:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Laser);
        break;
    case ESubWeapon::BladeArm:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Blade);
        break;
    case ESubWeapon::BusterCannonArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::BusterCannon);
        break;
    case ESubWeapon::DrillArm:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Drill);
        break;
    case ESubWeapon::HomingArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Homing);
        break;
    case ESubWeapon::HyperShellArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::HyperShell);
        break;
    case ESubWeapon::MachinegunArm:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Machinegun);
        break;
    case ESubWeapon::ShieldArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Shield);
        break;
    case ESubWeapon::SpreadBusterArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::SpreadBuster);
        break;
    default:
        break;
    }

    if (GI()->IsKey_Pressed(DIK_X))
    {
        m_ActionKey.Act(EActionKey::Squat);
    }
}

void CPlayer::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        if (nullptr == m_pGrabUnit)
            m_pModelComp->Set_Animation(0, 1.f, true);
        else
            m_pModelComp->Set_Animation(27, 0.f, false, true);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (!m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Jump);
        if (m_bIsMoving)
            m_State_Act.Set_State(EState_Act::Run);

        if (m_pGrabUnit == nullptr)
        {
            if (m_ActionKey.IsAct(EActionKey::Buster))
                m_State_Act.Set_State(EState_Act::Buster);
            if (m_ActionKey.IsAct(EActionKey::Laser))
                m_State_Act.Set_State(EState_Act::ReadyLaser);
            if (m_ActionKey.IsAct(EActionKey::Homing))
                m_State_Act.Set_State(EState_Act::Homing);
            if (m_ActionKey.IsAct(EActionKey::SpreadBuster))
                m_State_Act.Set_State(EState_Act::SpreadBuster);
            if (m_ActionKey.IsAct(EActionKey::Blade))
                m_State_Act.Set_State(EState_Act::BladeAttack1);
            if (m_ActionKey.IsAct(EActionKey::BusterCannon))
                m_State_Act.Set_State(EState_Act::ReadyBusterCannon);
            if (m_ActionKey.IsAct(EActionKey::HyperShell))
                m_State_Act.Set_State(EState_Act::ReadyHyperShell);
            if (m_ActionKey.IsAct(EActionKey::Machinegun))
                m_State_Act.Set_State(EState_Act::ReadyMachinegun);
        }
        else
        {
            if (m_ActionKey.IsAct(EActionKey::Throw))
                m_State_Act.Set_State(EState_Act::Throw);
            if (m_ActionKey.IsAct(EActionKey::Squat))
                m_State_Act.Set_State(EState_Act::Squat);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Run(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        switch (m_eMoveDir)
        {
        case MOVE_FORWARD:
            if (nullptr == m_pGrabUnit)
                m_pModelComp->Set_Animation(2, 1.0f, true);
            else
                m_pModelComp->Set_Animation(31, 1.0f, true);
            break;
        case MOVE_RIGHT:
            if (nullptr == m_pGrabUnit)
                m_pModelComp->Set_Animation(8, 1.0f, true);
            else
                m_pModelComp->Set_Animation(33, 1.0f, true);
            break;
        case MOVE_LEFT:
            if (nullptr == m_pGrabUnit)
                m_pModelComp->Set_Animation(9, 1.0f, true);
            else
                m_pModelComp->Set_Animation(34, 1.0f, true);
            break;
        case MOVE_BACK:
            if (nullptr == m_pGrabUnit)
                m_pModelComp->Set_Animation(10, 1.0f, true);
            else
                m_pModelComp->Set_Animation(32, 1.0f, true);
            break;
        }
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_ePrevMoveDir != m_eMoveDir)
        {
            switch (m_eMoveDir)
            {
            case MOVE_FORWARD:
                if (nullptr == m_pGrabUnit)
                    m_pModelComp->Set_AnimationMaintain(2, 1.0f, true);
                else
                    m_pModelComp->Set_AnimationMaintain(31, 1.0f, true);
                break;
            case MOVE_RIGHT:
                if (nullptr == m_pGrabUnit)
                    m_pModelComp->Set_AnimationMaintain(8, 1.0f, true);
                else
                    m_pModelComp->Set_AnimationMaintain(33, 1.0f, true);
                break;
            case MOVE_LEFT:
                if (nullptr == m_pGrabUnit)
                    m_pModelComp->Set_AnimationMaintain(9, 1.0f, true);
                else
                    m_pModelComp->Set_AnimationMaintain(34, 1.0f, true);
                break;
            case MOVE_BACK:
                if (nullptr == m_pGrabUnit)
                    m_pModelComp->Set_AnimationMaintain(10, 1.0f, true);
                else
                    m_pModelComp->Set_AnimationMaintain(32, 1.0f, true);
                break;
            }
        }

        if (m_fFootSound.Increase(fTimeDelta))
        {
            GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_run.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
            m_fFootSound.Reset();
        }

        if (!m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Jump);
        if (!m_bIsMoving)
            m_State_Act.Set_State(EState_Act::Idle);
        if (m_pGrabUnit == nullptr)
        {
            if (m_ActionKey.IsAct(EActionKey::Buster))
                m_State_Act.Set_State(EState_Act::Buster);
            if (m_ActionKey.IsAct(EActionKey::Laser))
                m_State_Act.Set_State(EState_Act::ReadyLaser);
            if (m_ActionKey.IsAct(EActionKey::Homing))
                m_State_Act.Set_State(EState_Act::Homing);
            if (m_ActionKey.IsAct(EActionKey::SpreadBuster))
                m_State_Act.Set_State(EState_Act::SpreadBuster);
            if (m_ActionKey.IsAct(EActionKey::Blade))
                m_State_Act.Set_State(EState_Act::BladeAttack1);
            if (m_ActionKey.IsAct(EActionKey::BusterCannon))
                m_State_Act.Set_State(EState_Act::ReadyBusterCannon);
            if (m_ActionKey.IsAct(EActionKey::HyperShell))
                m_State_Act.Set_State(EState_Act::ReadyHyperShell);
            if (m_ActionKey.IsAct(EActionKey::Machinegun))
                m_State_Act.Set_State(EState_Act::ReadyMachinegun);
        }
        else
        {
            if (m_ActionKey.IsAct(EActionKey::Throw))
                m_State_Act.Set_State(EState_Act::Throw);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Walk(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(2, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (!m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Jump);
        if (!m_bIsMoving)
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Ready_Jump(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(3, 1.f, false, false, 0.5f);
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_jump.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Jump_Up);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Jump_Up(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(4, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_vVelocity.y < 0)
            m_State_Act.Set_State(EState_Act::Jump_Down);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Jump_Down(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(5, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Landing);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Landing(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(6, 1.f, false, false, 0.3f);
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_landing.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        Input_Move(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Buster(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(7, 2.f, false, false, 0.8f);
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_buster.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
        ShootBuster();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            if (GI()->IsMouse_Pressing(DIM_LB))
                m_State_Act.Set_State(EState_Act::Buster);
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_DamagedLight(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(14, 1.f, false, false, 0.5f);
    }

    if (m_State_Act.Can_Update())
    {
        Transform().MoveForward(-fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_DamagedHeavy(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(15, 1.f, false, false, 0.3f);
        m_vVelocity.y = 3.f;
        m_vVelocity.z = -3.f;
    }

    if (m_State_Act.Can_Update())
    {
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::KnockDown);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_KnockDown(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(16, 1.f, false, false, 0.1f);
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
        m_bInvisible = true;
    }

    if (m_State_Act.Can_Update())
    {
        m_fInvisibleTime.Reset();
        if (m_fKnockDownTime.Increase(fTimeDelta))
        {
            m_fKnockDownTime.Reset();
            m_State_Act.Set_State(EState_Act::StandUp);
        }
    }

    if (m_State_Act.IsState_Exit())
    {
        m_fKnockDownValue.Reset();
    }
}

void CPlayer::ActState_StandUp(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(17, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ReadyLaser(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(18, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::ShootingLaser);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ShootingLaser(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(19, 1.f, true, false, 0.2f);
        ShootLaser();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        AttachLaser();

        if (!GI()->IsMouse_Pressing(DIM_RB))
        {
            m_State_Act.Set_State(EState_Act::EndLaser);
        }
    }

    if (m_State_Act.IsState_Exit())
    {
        DeleteLaser();
    }
}

void CPlayer::ActState_EndLaser(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(18, 1.f, false, true, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Homing(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(19, 1.f, false, false, 0.2f);
        ShootMissile();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_SpreadBuster(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(19, 1.f, false, false, 0.2f);
        ShootSpreadBuster();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ChargeShot(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(19, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ReadyBusterCannon(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(24, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::ShootBusterCannon);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ShootBusterCannon(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(25, 1.f, false, false, 0.2f);
        m_ActionKey.Act(EActionKey::MoveBackward);
        ShootBusterCannon();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        m_ActionKey.Act(EActionKey::LowFrict);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::EndBusterCannon);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_EndBusterCannon(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(24, 1.f, false, true, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ReadyHyperShell(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(24, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::ShootHyperShell);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ShootHyperShell(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(25, 1.f, false, false, 0.2f);
        m_ActionKey.Act(EActionKey::MoveBackward);
        ShootHyperShell();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        m_ActionKey.Act(EActionKey::LowFrict);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::EndHyperShell);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_EndHyperShell(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(24, 1.f, false, true, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ReadyMachinegun(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(22, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::ShootingMachinegun);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ShootingMachinegun(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(23, 1.f, false, false, 0.2f);
        ShootMachinegun();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (!m_ActionKey.IsAct(EActionKey::Machinegun))
            m_State_Act.Set_State(EState_Act::EndMachinegun);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::ShootingMachinegun);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_EndMachinegun(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(22, 1.f, false, true, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_BladeAttack1(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(20, 1.5f, false, false, 0.2f);
        m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("blade_attack1.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
        m_ActionKey.Act(EActionKey::MoveForward);
        m_ActionKey.Act(EActionKey::MoveFast);
        CreateBlade();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        AttachBlade();
        m_ActionKey.Act(EActionKey::LowFrict);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            if (m_ActionKey.IsAct(EActionKey::Blade))
                m_State_Act.Set_State(EState_Act::BladeAttack2);
            else
                m_State_Act.Set_State(EState_Act::BladeEnd);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_BladeAttack2(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(21, 1.3f, false, false, 0.2f);
        m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("blade_attack1.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
        m_fGauge.Readjust(0.2f);
        
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        AttachBlade();
        m_fGauge.Increase(fTimeDelta);
        if (m_fGauge.IsMax_Once())
        {
            m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_jump.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
            m_ActionKey.Act(EActionKey::Jump);
        }

        if (m_pModelComp->AnimationComp()->IsAnimation_Range(34.f, 35.f))
        {
            m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("blade_attack2.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
        }

        m_ActionKey.Act(EActionKey::MoveForward);
        m_ActionKey.Act(EActionKey::JumpLow);

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            if (m_bIsOnGround)
                m_State_Act.Set_State(EState_Act::BladeEnd);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_BladeEnd(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, false, false, 0.2f);
        DeleteBlade();
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Grab(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(26, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Grabbing);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Grabbing(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(27, 1.f, false, false, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Throw(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(28, 1.f, false, false, 0.2f);
        
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);
        if (m_pModelComp->AnimationComp()->IsAnimation_UpTo(13.f))
            ThrowUnit();

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_Squat(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(36, 1.f, false, false, 0.2f);

    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (m_pModelComp->AnimationComp()->IsAnimation_Range(9.5f, 10.5f))
        {
            if (m_pGrabUnit)
            {
                // 핑거리 일 때만 이벤트가 발생한다.
                auto pFingerii = DynCast<CReaverBot_Fingerii*>(m_pGrabUnit);
                if (pFingerii)
                {
                    pFingerii->SquatBonus();
                }
            }
        }

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ItemGetting(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, false, true, 0.2f);
    }

    if (m_State_Act.Can_Update())
    {

    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ActState_ItemGet(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(30, 1.f, false, false, 0.2f);

    }

    if (m_State_Act.Can_Update())
    {

    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CPlayer::ShootBuster()
{
    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(110);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.1f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pBuster = CWeapon_Buster::Create(vfPos);
    if (pBuster == nullptr)
        return;

    _vector vLook = Calculate_ShootLookVector(vPos);

    GI()->Add_GameObject(pBuster);
    pBuster->Set_LifeTime(1.f);
    pBuster->Set_Speed(20.f);
    pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + vLook);
    pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
}

void CPlayer::ShootMissile()
{
    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.4f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pMissile = CWeapon_HomingMissile::Create(vfPos);
    if (pMissile == nullptr)
        return;

    // 초기에만 세팅
    if (m_pLockon_Target)
        pMissile->Set_Target(m_pLockon_Target);

    _vector vLook = Calculate_ShootLookVector(vPos);

    pMissile->Set_Speed(20.f);
    pMissile->Set_LifeTime(1.f);
    pMissile->Transform().Look_At(pMissile->Transform().Get_PositionVector() + vLook);
    pMissile->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
    m_pGI->Add_GameObject(pMissile);

    m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("homming_shoot.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
}

void CPlayer::ShootSpreadBuster()
{
    for (_uint i = 0; i < 5; i++)
    {
        _vector vPos = m_pModelComp->Get_BoneTransformMatrixWithParents(135).r[3];
        _float3 vfPos = {};
        XMStoreFloat3(&vfPos, vPos);
        auto pBuster = CWeapon_SpreadBuster::Create(vfPos);
        if (pBuster == nullptr)
            return;

        _vector vLook = Calculate_ShootLookVector(vPos);

        _int iCont = i - 2;
        _float fRadian = XMConvertToRadians(Cast<_float>(iCont * 20));

        pBuster->Set_LifeTime(0.6f);
        pBuster->Set_Speed(30.f);
        pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + vLook);
        pBuster->Transform().TurnRight(fRadian);
        pBuster->Transform().MoveForward(0.5f);
        pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
        m_pGI->Add_GameObject(pBuster);
    }
    m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("spreadgun_shoot.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
}

void CPlayer::ShootLaser()
{
    if (m_pLaserEmission != nullptr)
        return;

    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.4f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    m_pLaserEmission = CWeapon_LaserEmission::Create(vfPos);
    if (m_pLaserEmission == nullptr)
        return;

    _vector vLook = Calculate_ShootLookVector(vPos);

    // 초기에만 세팅
    m_pLaserEmission->Transform().Look_At(m_pLaserEmission->Transform().Get_PositionVector() + vLook);
    m_pLaserEmission->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
    m_pGI->Add_GameObject(m_pLaserEmission);
    Safe_AddRef(m_pLaserEmission);
}

void CPlayer::AttachLaser()
{
    if (m_pLaserEmission)
    {
        _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
        _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.4f;
        _vector vLook = Calculate_ShootLookVector(vPos);
        m_pLaserEmission->Transform().Set_Position(vPos);
        m_pLaserEmission->Transform().Look_At(m_pLaserEmission->Transform().Get_PositionVector() + vLook);
    }
}

void CPlayer::DeleteLaser()
{
    if (m_pLaserEmission)
    {
        m_pLaserEmission->EndShooting();
        Safe_ReleaseAndUnlink(m_pLaserEmission);
    }
}

void CPlayer::ShootBusterCannon()
{
    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.9f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pBuster = CWeapon_BusterCannon::Create(vfPos);
    if (pBuster == nullptr)
        return;

    _vector vLook = Calculate_ShootLookVector(vPos);

    m_pGI->Add_GameObject(pBuster);
    pBuster->Set_LifeTime(1.f);
    pBuster->Set_Speed(80.f);
    pBuster->Set_Damage(5.f);
    pBuster->Transform().Set_Scale(1.1f, 1.1f, 1.1f);
    pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + vLook);
    pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());

    m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("buster_cannon_shoot.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
}

void CPlayer::ShootHyperShell()
{
    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.9f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pBuster = CWeapon_HyperShell::Create(vfPos);
    if (pBuster == nullptr)
        return;

    _vector vLook = Calculate_ShootLookVector(vPos);

    m_pGI->Add_GameObject(pBuster);
    pBuster->Set_LifeTime(1.5f);
    pBuster->Set_Speed(0.01f);
    pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + vLook);
    pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());

    m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("hyper_shell_shoot.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
}

void CPlayer::ShootMachinegun()
{
    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3] + XMVector3Normalize(BoneMatrix.r[1]) * 0.4f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pMachinegun = CWeapon_Machinegun::Create(vfPos);
    if (pMachinegun == nullptr)
        return;

    _vector vLook = Calculate_ShootLookVector(vPos);

    m_pGI->Add_GameObject(pMachinegun);
    pMachinegun->Set_LifeTime(0.8f);
    pMachinegun->Set_Speed(30.f);
    pMachinegun->Transform().Look_At(pMachinegun->Transform().Get_PositionVector() + vLook);
    pMachinegun->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());

    m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("machinegun_shoot.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
}

void CPlayer::CreateBlade()
{
    if (m_pBlade)
        return;

    _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
    _vector vPos = BoneMatrix.r[3];// +XMVector3Normalize(BoneMatrix.r[1]);
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    m_pBlade = CWeapon_Blade::Create(vfPos);
    if (m_pBlade == nullptr)
        return;

    GI()->Add_GameObject(m_pBlade);
    m_pBlade->Transform().Set_Transform(BoneMatrix);
    m_pBlade->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
    Safe_AddRef(m_pBlade);
}

void CPlayer::AttachBlade()
{
    if (m_pBlade)
    {
        _matrix BoneMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
        //BoneMatrix.r[3] += XMVector3Normalize(BoneMatrix.r[1]);
        m_pBlade->Transform().Set_Transform(BoneMatrix);
    }
}

void CPlayer::DeleteBlade()
{
    if (m_pBlade)
    {
        m_pBlade->Set_Dead();
        Safe_ReleaseAndUnlink(m_pBlade);
    }
}

void CPlayer::GrabingUnit()
{
    if (m_pGrabUnit)
    {
        _matrix RightHandMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(135);
        _matrix LeftHandMatrix = m_pModelComp->Get_BoneTransformMatrixWithParents(110);

        _vector vRightHandLook = XMVector3Normalize(RightHandMatrix.r[0]);
        _vector vLeftHandLook = XMVector3Normalize(LeftHandMatrix.r[0]);
        _vector vLook = XMVector3Normalize(XMVectorLerp(vRightHandLook, -vLeftHandLook, 0.5f));

        _vector vPos = (RightHandMatrix.r[3] + LeftHandMatrix.r[3]) * 0.5f;
        m_pGrabUnit->Transform().Set_Position(vPos);
        m_pGrabUnit->Transform().Look_At(vPos + vLook);
    }
}

void CPlayer::ThrowUnit()
{
    if (m_pGrabUnit)
    {
        _vector vLook = Transform().Get_LookNormalizedVector() * 5.f;
        _float3 vfVelocity = {};
        XMStoreFloat3(&vfVelocity, vLook);
        vfVelocity.y = 3.f;
        m_pGrabUnit->Set_Velocity(vfVelocity);
        m_pGrabUnit->Set_Grabbed(false);
        m_pGrabUnit->Set_Throwing(true);
        m_pGrabUnit->Transform().Look_At_OnLand(Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector());

        Safe_ReleaseAndUnlink(m_pGrabUnit);

        m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_throw.mp3"), CHANNELID::SOUND_COMMON1, 1.f);
    }
}

void CPlayer::Gotcha(_bool bIsGetItem, EItemObtain eItem)
{
    if (bIsGetItem)
    {
        if (eItem == EItemObtain::SubWeapons)
            m_bIsCanUseSubWeapons = true;
        m_State_Act.Set_State(EState_Act::ItemGet);
    }
    else
        m_State_Act.Set_State(EState_Act::Idle);
}

void CPlayer::EndOpenChest()
{
    m_State_Act.Set_State(EState_Act::Idle);
}

void CPlayer::Lockon_Active(const _float& fTimeDelta)
{
    if (m_pLockon_Target)
    {
        if (m_pLockon_Target == m_pGrabUnit || m_pLockon_Target->IsDead() || GI()->IsKey_Pressed(DIK_R))
        {
            Lockon_Untarget();
            return;
        }

        Transform().Look_At_OnLand(m_pLockon_Target->Transform().Get_PositionVector(), 10.f * fTimeDelta);
    }
    else
    {
        if (GI()->IsKey_Pressed(DIK_R))
            Lockon_Target();
    }
}

void CPlayer::Lockon_Target()
{
    if (nullptr != m_pLockon_Target)
        return;

    m_pLockon_Target = Find_Target(20.f);
    if (nullptr == m_pLockon_Target)
        return;

    Safe_AddRef(m_pLockon_Target);

    CUI_Lockon* pLockon = CUI_Lockon::Create();
    if (FAILED(GI()->Add_GameObject(pLockon)))
        return;

    if (nullptr == pLockon)
        return;

    pLockon->Transform().Set_Position(PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One));
    pLockon->Set_Target(m_pLockon_Target);

    m_pLockon_UI = pLockon;
    Safe_AddRef(m_pLockon_UI);

    GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("lockon.mp3"), CHANNELID::SOUND_SYSTEM1, 1.f);
}

void CPlayer::Lockon_Untarget()
{
    Safe_ReleaseAndUnlink(m_pLockon_Target);

    m_pLockon_UI->Clear_Target();
    Safe_ReleaseAndUnlink(m_pLockon_UI);
}

_matrix CPlayer::Lockon_TargetMatrix()
{
    return m_pLockon_Target->ColliderComp()->Calculate_TransformMatrixFromParent();
}

_vector CPlayer::Camera_LookVector()
{
    _vector vLook = {};
    _vector vRight = Transform().Get_RightNormalizedVector();
    _vector qtRotate = XMQuaternionRotationAxis(vRight, XMConvertToRadians(-5.f));
    vLook = m_pDynamicCamera->Transform().Get_LookNormalizedVector();
    vLook = XMVector3Normalize(XMVector3Rotate(vLook, qtRotate));

    return vLook;
}

_vector CPlayer::Calculate_ShootLookVector(_fvector vPos)
{
    _vector vLook = Transform().Get_LookNormalizedVector();
    if (m_pLockon_Target)
    {
        _matrix TargetMatrix = Lockon_TargetMatrix();
        vLook = XMVector3Normalize(TargetMatrix.r[3] - vPos);
    }
    else if (m_pDynamicCamera)
    {
        vLook = Camera_LookVector();
    }

    return vLook;
}

CCharacter_Common* CPlayer::Find_Target(_float fRange)
{
    auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), fRange, COLLAYER_CHARACTER);
    _float fDistance = FLT_MAX;
    CGameObject* pClosestObj = { nullptr };
    for (auto iter = listObjects.begin(); iter != listObjects.end(); iter++)
    {
        auto pObj = iter->first;
        if (pObj == m_pGrabUnit)
            continue;

        auto& ContactData = iter->second;
        _float fObjDistance = XMVectorGetX(XMVector3Length((Transform().Get_PositionVector() - pObj->Transform().Get_PositionVector())));
        if (fObjDistance <= fDistance
            && pObj != this && nullptr != DynCast<CCharacter_Common*>(pObj))
        {
            if (ETeamRelation::Hostile ==
                CTeamAgentComp::Check_Relation(&DynCast<CCharacter_Common*>(pObj)->TeamAgentComp(), &TeamAgentComp()))
            {
                fDistance = fObjDistance;
                pClosestObj = pObj;
            }

        }
    }

    return DynCast<CCharacter_Common*>(pClosestObj);
}

void CPlayer::ChangeMainWeapon(EMainWeapon eWeapon)
{
    m_eMainWeapon = eWeapon;

    m_pModelComp->Deactive_Mesh(9);
    m_pModelComp->Deactive_Mesh(24);
    m_pModelComp->Deactive_Mesh(19);

    switch (m_eMainWeapon)
    {
    case Client::EMainWeapon::None:
        m_pModelComp->Active_Mesh(9);
        m_pModelComp->Active_Mesh(24);
        break;
    case Client::EMainWeapon::Buster:
        m_pModelComp->Active_Mesh(19);
        break;
    case Client::EMainWeapon::ChargeBuster:
        m_pModelComp->Active_Mesh(19);
        break;
    default:
        break;
    }
}

void CPlayer::ChangeSubWeapon(ESubWeapon eWeapon)
{
    m_eSubWeapon = eWeapon;

    m_pModelComp->Deactive_Mesh(6);
    m_pModelComp->Deactive_Mesh(7);
    m_pModelComp->Deactive_Mesh(11);
    m_pModelComp->Deactive_Mesh(15);
    m_pModelComp->Deactive_Mesh(16);
    m_pModelComp->Deactive_Mesh(17);
    m_pModelComp->Deactive_Mesh(18);
    m_pModelComp->Deactive_Mesh(20);
    m_pModelComp->Deactive_Mesh(21);
    m_pModelComp->Deactive_Mesh(22);
    m_pModelComp->Deactive_Mesh(23);

    switch (m_eSubWeapon)
    {
    case Client::ESubWeapon::ThrowArm:
        m_pModelComp->Active_Mesh(6);
        m_pModelComp->Active_Mesh(7);
        break;
    case Client::ESubWeapon::LaserArm:
        m_pModelComp->Active_Mesh(11);
        break;
    case Client::ESubWeapon::BladeArm:
        m_pModelComp->Active_Mesh(18);
        break;
    case Client::ESubWeapon::BusterCannonArm:
        m_pModelComp->Active_Mesh(20);
        break;
    case Client::ESubWeapon::DrillArm:
        m_pModelComp->Active_Mesh(21);
        break;
    case Client::ESubWeapon::HomingArm:
        m_pModelComp->Active_Mesh(22);
        break;
    case Client::ESubWeapon::HyperShellArm:
        m_pModelComp->Active_Mesh(23);
        break;
    case Client::ESubWeapon::MachinegunArm:
        m_pModelComp->Active_Mesh(15);
        break;
    case Client::ESubWeapon::ShieldArm:
        m_pModelComp->Active_Mesh(16);
        break;
    case Client::ESubWeapon::SpreadBusterArm:
        m_pModelComp->Active_Mesh(17);
        break;
    default:
        break;
    }
}


