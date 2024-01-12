#include "GameObject/Player.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/NavigationComponent.h"
#include "GameObject/Weapon_Buster.h"
#include "Component/ColliderComponent.h"

#include "System/RenderMgr.h"
#include "Utility/ClassID.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/UI_Lockon.h"

CPlayer::CPlayer()
{
    Set_Name(L"Player");
    Register_State();
}

CPlayer::CPlayer(const CPlayer& rhs)
    : Base(rhs)
    , m_pModelComp(rhs.m_pModelComp)
{
    Register_State();
}

HRESULT CPlayer::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

HRESULT CPlayer::Initialize_Prototype(const _float3 vPos)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    Transform().Set_Position(vPos);
    m_vAcceleration = m_vMoveSpeed = m_vMaxMoveSpeed = { 6.f, 10.f, 6.f };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

HRESULT CPlayer::Initialize_Prototype(FSerialData& Data)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    _float3 vPos = {};
    if (FAILED(Data.Get_Data("PosX", vPos.x)))
        return E_FAIL;
    if (FAILED(Data.Get_Data("PosY", vPos.y)))
        return E_FAIL;
    if (FAILED(Data.Get_Data("PosZ", vPos.z)))
        return E_FAIL;

    Transform().Set_Position(vPos);
    m_vAcceleration = m_vMoveSpeed = m_vMaxMoveSpeed = { 6.f, 10.f, 6.f };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

HRESULT CPlayer::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

HRESULT CPlayer::Initialize(const _float3 vPos)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    Transform().Set_Position(vPos);
    m_vAcceleration = m_vMoveSpeed = m_vMaxMoveSpeed = { 6.f, g_fGravity, 6.f };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };
    
    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (m_fInvisibleTime.Increase(fTimeDelta))
        m_bInvisible = false;

    m_fKnockDownDelay.Increase(fTimeDelta);
    // 넉다운 수치 시간당 감소
    m_fKnockDownValue.Increase(-fTimeDelta * 0.1f);

    Lockon_Active(fTimeDelta);

    m_State_Act.Get_StateFunc()(this, fTimeDelta);

    m_pColliderComp->Tick(fTimeDelta);
    m_pCameraPivotComp->Tick(fTimeDelta);
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();
    m_pModelComp->Invalidate_BoneTransforms();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
    SUPER::Render();
    
    m_pModelComp->Render();

#ifdef _DEBUG
    m_pColliderComp->Render();
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

CPlayer* CPlayer::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CPlayer* CPlayer::Create(FSerialData& Data)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(Data)))
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

void CPlayer::Free()
{
    SUPER::Free();

    Safe_Release(m_pLockon_Target);
    Safe_Release(m_pLockon_UI);
}

FSerialData CPlayer::SerializeData()
{
    FSerialData Data = __super::SerializeData();

    Data.Add_Member("ClassID", ECast(ThisClass::g_ClassID));
    Data.Add_Member("HP", m_fHP.fMax);
    Data.Add_Member("MoveSpeed", m_vMaxMoveSpeed.x);
    Data.Add_Member("JumpSpeed", m_vMaxMoveSpeed.y);

    return Data;
}

HRESULT CPlayer::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    FAILED_CHECK_RETURN(Add_Component(L"CameraPivot", m_pCameraPivotComp = CPivotComponent::Create()), E_FAIL);
    m_pCameraPivotComp->OffsetTransform().Set_Position(0.f, 1.3f, 0.f);

    m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/RockVolnutt.amodel");

    /*m_pModelComp->Create_Mask(L"Main", L"Armature", true);
    m_pModelComp->Create_Mask(L"Head", L"Armature", false);
    m_pModelComp->Create_Mask(L"Leg", L"Armature", false);
    m_pModelComp->Create_Mask(L"LeftArm", L"Armature", false);
    m_pModelComp->Create_Mask(L"RightArm", L"Armature", false);

    m_pModelComp->Active_BoneMask(2, L"bone_000");*/

    CNavigationComponent::TCloneDesc tDesc = { 0 };
    FAILED_CHECK_RETURN(Add_Component(L"Navigation",
        m_pNaviComp = Cast<CNavigationComponent*>(GI()->Clone_PrototypeComp(TEXT("Prototype_Component_Navigation"), VPCast(&tDesc)))), E_FAIL);

    if (nullptr == m_pColliderComp)
        return E_FAIL;
    m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
                                        | COLLAYER_ITEM | COLLAYER_OBJECT);

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
            }

            //Create_Effect();
        }
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



void CPlayer::Register_State()
{
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

    /*_vector vMoveDir = XMVector3Normalize(XMLoadFloat3(&m_vLookDirection));
    _vector vCurMoveDir = XMVector3Normalize(XMLoadFloat3(&m_vLookDirection_Blend));
    _float fObjAngle = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), vMoveDir));
    _float fCurAngle = XMVectorGetX(XMVector3Dot(XMVectorSet(0.f, 0.f, 1.f, 0.f), vCurMoveDir));
    _vector vObjRotate = XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fObjAngle);
    _vector vCurRotate = XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fCurAngle);
    _vector vBlended_Rotate = XMQuaternionSlerp(vObjRotate, vCurRotate, 0.5f);
    _float fFinalAngle = acosf(XMVectorGetX(XMVector4Dot(vCurRotate, vBlended_Rotate)));
    vCurMoveDir = XMVector3Rotate(vCurMoveDir, XMQuaternionRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fFinalAngle));
    XMStoreFloat3(&m_vLookDirection_Blend, vCurMoveDir);*/

    //Transform().TurnAxis(_float3(0.f, 1.f, 0.f), m_vLookDirection.x * 5.f * fTimeDelta);

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
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_LB))
            m_State_Act.Set_State(EState_Act::Buster);
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_State_Act.Set_State(EState_Act::ReadyLaser);
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
        if (GI()->IsMouse_Pressed(MOUSEKEYSTATE::DIM_LB))
            m_State_Act.Set_State(EState_Act::Buster);
        if (GI()->IsMouse_Pressing(MOUSEKEYSTATE::DIM_RB))
            m_State_Act.Set_State(EState_Act::ReadyLaser);
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

void CPlayer::ShootBuster()
{
    _vector vPos = Transform().Get_PositionVector() + XMVectorSet(0.f, 0.5f, 0.f, 0.f);
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

void CPlayer::Lockon_Active(const _float& fTimeDelta)
{
    if (m_pLockon_Target)
    {
        if (m_pLockon_Target->IsDead() || GI()->IsKey_Pressed(DIK_R))
        {
            Lockon_Untarget();
            return;
        }

        Transform().Look_At_OnLand(m_pLockon_Target->Transform().Get_PositionVector());//, 10.f * fTimeDelta);
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


