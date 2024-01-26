#include "GameObject/Player.h"

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

#include "CloudStation/CloudStation_Player.h"

#include "System/RenderMgr.h"


CPlayer::CPlayer()
{
    Set_Name(L"Player");
    Register_State();
}

CPlayer::CPlayer(const CPlayer& rhs)
    : Base(rhs)
{
    Register_State();
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

    if (nullptr == m_pPlayerCloud && nullptr != m_pCloudStationComp)
    {
        if (SUCCEEDED(m_pCloudStationComp->Connect_CloudStation(TEXT("Player"))))
        {
            m_pPlayerCloud = m_pCloudStationComp->Get_LastCloudStation<CCloudStation_Player>();
            Safe_AddRef(m_pPlayerCloud);
        }
    }
}

void CPlayer::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (m_fInvisibleTime.Increase(fTimeDelta))
        m_bInvisible = false;

    m_fKnockDownDelay.Increase(fTimeDelta);
    // 넉다운 수치 시간당 감소
    m_fKnockDownValue.Increase(-fTimeDelta);

    Lockon_Active(fTimeDelta);

    Input_Weapon(fTimeDelta);
    m_State_Act.Get_StateFunc()(this, fTimeDelta);
    m_ActionKey.Reset();

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
            m_pColliderComp->EnterToPhysics(0);
            break;
        case ECast(EComponentID::CloudStation):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pCloudStationComp = DynCast<CCloudStationComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pCloudStationComp->Open_CloudStation_Session(TEXT("Player"), CCloudStation_Player::Create());
            m_pCloudStationComp->Connect_CloudStation(TEXT("Player"));
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

    CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
    if (pEnemy)
    {
        if (CTeamAgentComp::ERelation::Hostile ==
            CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
        {
            if (m_fKnockDownDelay.IsMax() && !m_bInvisible)
            {
                m_fKnockDownDelay.Reset();
                if (m_fKnockDownValue.Increase(2.5f))
                {
                    m_State_Act.Set_State(EState_Act::DamagedHeavy);
                    GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
                }
                else
                {
                    m_State_Act.Set_State(EState_Act::DamagedLight);
                    GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
                }
                m_fHP.Increase(-5.f);
            }

            //Create_Effect();
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
                GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
                m_fHP.Increase(-5.f);
            }

            //Create_Effect();
        }
    }

    CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
    if (pDoor)
    {
        if (GI()->IsKey_Pressed(DIK_E))
            pDoor->OpenDoor();

        _float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
        _vector vSimNormal = {};
        vSimNormal = XMLoadFloat3(&vNormal);
        Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
        if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
            m_bIsOnGround = true;
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
    if (GI()->IsKey_Pressing(DIK_W))
    {
        m_vLookDirection.z = 1.f;
        m_vVelocity.z += m_vAcceleration.z * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_FORWARD;
    }
    else if (GI()->IsKey_Pressing(DIK_S))
    {
        m_vLookDirection.z = -1.f;
        m_vVelocity.z -= m_vAcceleration.z * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_BACK;
    }
    else
    {
        if (m_bIsOnGround)
            m_vVelocity.z *= 0.5f;
        else
            m_vVelocity.z *= 0.998f;
    }

    if (GI()->IsKey_Pressing(DIK_D))
    {
        m_vLookDirection.x = 1.f;
        m_vVelocity.x += m_vAcceleration.x * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_RIGHT;
    }
    else if (GI()->IsKey_Pressing(DIK_A))
    {
        m_vLookDirection.x = -1.f;
        m_vVelocity.x -= m_vAcceleration.x * fTimeDelta;
        m_bIsMoving = true;
        m_eMoveDir = MOVE_LEFT;
    }
    else
    {
        if (m_bIsOnGround)
            m_vVelocity.x *= 0.5f;
        else
            m_vVelocity.z *= 0.998f;
    }

    if (m_bIsOnGround)
    {
        if (GI()->IsKey_Pressed(DIK_SPACE))
        {
            m_vVelocity.y = m_vMoveSpeed.y;
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
    Transform().TurnAxis(_float3(0.f, 1.f, 0.f), Cast<_float>(GI()->Get_DIMouseMove(DIMS_X)) * 0.1f * fTimeDelta);
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

    if (GI()->IsKey_Pressed(DIK_C))
    {
        if (ECast(ESubWeapon::SpreadBusterArm) == ECast(m_eSubWeapon))
            ChangeSubWeapon(ESubWeapon::ThrowArm);
        else
            ChangeSubWeapon(Cast<ESubWeapon>(ECast(m_eSubWeapon) + 1));
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
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Throw);
        break;
    case ESubWeapon::LaserArm:
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_ActionKey.Act(EActionKey::Laser);
        break;
    case ESubWeapon::BladeArm:
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
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
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_RB))
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
}

void CPlayer::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);
        Look_Update(fTimeDelta);

        if (!m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Jump);
        if (m_bIsMoving)
            m_State_Act.Set_State(EState_Act::Run);
        if (m_ActionKey.IsAct(EActionKey::Buster))
            m_State_Act.Set_State(EState_Act::Buster);
        if (m_ActionKey.IsAct(EActionKey::Laser))
            m_State_Act.Set_State(EState_Act::ReadyLaser);
        if (m_ActionKey.IsAct(EActionKey::Homing))
            m_State_Act.Set_State(EState_Act::Homing);
        if (m_ActionKey.IsAct(EActionKey::SpreadBuster))
            m_State_Act.Set_State(EState_Act::SpreadBuster);
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
            m_pModelComp->Set_Animation(2, 1.0f, true);
            break;
        case MOVE_RIGHT:
            m_pModelComp->Set_Animation(8, 1.0f, true);
            break;
        case MOVE_LEFT:
            m_pModelComp->Set_Animation(9, 1.0f, true);
            break;
        case MOVE_BACK:
            m_pModelComp->Set_Animation(10, 1.0f, true);
            break;
        }
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);
        Look_Update(fTimeDelta);

        if (m_ePrevMoveDir != m_eMoveDir)
        {
            switch (m_eMoveDir)
            {
            case MOVE_FORWARD:
                m_pModelComp->Set_AnimationMaintain(2, 1.0f, true);
                break;
            case MOVE_RIGHT:
                m_pModelComp->Set_AnimationMaintain(8, 1.0f, true);
                break;
            case MOVE_LEFT:
                m_pModelComp->Set_AnimationMaintain(9, 1.0f, true);
                break;
            case MOVE_BACK:
                m_pModelComp->Set_AnimationMaintain(10, 1.0f, true);
                break;
            }
        }

        if (m_fFootSound.Increase(fTimeDelta))
        {
            GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_run.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
            m_fFootSound.Reset();
        }

        if (!m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Jump);
        if (!m_bIsMoving)
            m_State_Act.Set_State(EState_Act::Idle);
        if (m_ActionKey.IsAct(EActionKey::Buster))
            m_State_Act.Set_State(EState_Act::Buster);
        if (m_ActionKey.IsAct(EActionKey::Laser))
            m_State_Act.Set_State(EState_Act::ReadyLaser);
        if (m_ActionKey.IsAct(EActionKey::Homing))
            m_State_Act.Set_State(EState_Act::Homing);
        if (m_ActionKey.IsAct(EActionKey::SpreadBuster))
            m_State_Act.Set_State(EState_Act::SpreadBuster);
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
        Move_Update(fTimeDelta);
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
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_jump.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);
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
        Move_Update(fTimeDelta);
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
        Move_Update(fTimeDelta);
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
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_landing.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);
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
        m_pModelComp->Set_Animation(7, 2.f, false, false, 0.5f);
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_buster.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
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
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);
        Transform().MoveForward(-4.f * fTimeDelta);

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
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_hit_strong.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
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
    }

    if (m_State_Act.Can_Update())
    {
        Look_Update(fTimeDelta);

        if (!GI()->IsMouse_Pressing(DIM_RB))
        {
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

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
        ShootBuster();
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

void CPlayer::ShootBuster()
{
    _vector vPos = Transform().Get_PositionVector() + XMVectorSet(0.f, 0.8f, 0.f, 0.f);
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pBuster = CWeapon_Buster::Create(vfPos);
    if (pBuster == nullptr)
        return;

    GI()->Add_GameObject(pBuster);
    pBuster->Set_LifeTime(1.f);
    pBuster->Set_Speed(20.f);
    pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector());
    pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
}

void CPlayer::ShootMissile()
{

}

void CPlayer::ShootSpreadBuster()
{
    for (_uint i = 0; i < 5; i++)
    {
        _vector vPos = Transform().Get_PositionVector() + XMVectorSet(0.f, 0.8f, 0.f, 0.f);
        _float3 vfPos = {};
        XMStoreFloat3(&vfPos, vPos);
        auto pBuster = CWeapon_Buster::Create(vfPos);
        if (pBuster == nullptr)
            return;

        _int iCont = i - 2;
        _float fRadian = XMConvertToRadians(Cast<_float>(iCont * 20));

        pBuster->Set_LifeTime(0.3f);
        pBuster->Set_Speed(20.f);
        pBuster->Transform().Look_At(pBuster->Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector());
        pBuster->Transform().TurnRight(fRadian);
        pBuster->Transform().MoveForward(0.5f);
        pBuster->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
        GI()->Add_GameObject(pBuster);
    }
}

void CPlayer::Lockon_Active(const _float& fTimeDelta)
{
    if (m_pLockon_Target)
    {
        if (m_pLockon_Target->IsDead() || GI()->IsKey_Pressed(DIK_R))
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

    m_pLockon_Target = Find_Target();
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

    GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("lockon.mp3"), CHANNELID::SOUND_SYSTEM_EFFECT, 1.f);
}

void CPlayer::Lockon_Untarget()
{
    Safe_Release(m_pLockon_Target);
    m_pLockon_Target = nullptr;

    m_pLockon_UI->Clear_Target();
    Safe_Release(m_pLockon_UI);
    m_pLockon_UI = nullptr;
}

CCharacter_Common* CPlayer::Find_Target()
{
    auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), 10.f, COLLAYER_CHARACTER);
    _float fDistance = FLT_MAX;
    CGameObject* pClosestObj = { nullptr };
    for (auto iter = listObjects.begin(); iter != listObjects.end(); iter++)
    {
        auto pObj = iter->first;
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


