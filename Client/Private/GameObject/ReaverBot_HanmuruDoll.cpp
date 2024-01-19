#include "GameObject/ReaverBot_HanmuruDoll.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/DamageCollision.h"

CReaverBot_HanmuruDoll::CReaverBot_HanmuruDoll()
{
    Set_Name(TEXT("ReaverBot_HanmuruDoll"));
    Set_RenderGroup(ERenderGroup::NonBlend);
    Register_State();
    m_fHP = FGauge(20.f, true);
    m_RandomNumber = mt19937_64(m_RandomDevice());
}

CReaverBot_HanmuruDoll::CReaverBot_HanmuruDoll(const CReaverBot_HanmuruDoll& rhs)
    : Base(rhs)
{
    Register_State();
    m_RandomNumber = mt19937_64(m_RandomDevice());
}

HRESULT CReaverBot_HanmuruDoll::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(1.f, 1.f, 1.f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/HanmuruDoll/HanmuruDoll.amodel");
    m_pModelComp->Set_Animation(0, 1.f, true);

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
        | COLLAYER_ATTACKER | COLLAYER_OBJECT);

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

HRESULT CReaverBot_HanmuruDoll::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

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
            m_pColliderComp->EnterToPhysics(0);
            break;
        }
    }

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

HRESULT CReaverBot_HanmuruDoll::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;


    return S_OK;
}

HRESULT CReaverBot_HanmuruDoll::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CReaverBot_HanmuruDoll::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_HanmuruDoll::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (!m_State_Act.IsOnState(EState_Act::Dead) && m_fHP.Get_Percent() <= 0.f)
        m_State_Act.Set_State(EState_Act::Dead);

    //Input_ActionKey();
    m_State_AI.Get_StateFunc()(this, fTimeDelta);
    m_State_Act.Get_StateFunc()(this, fTimeDelta);
    m_ActionKey.Reset();

    m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_HanmuruDoll::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_HanmuruDoll::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

FSerialData CReaverBot_HanmuruDoll::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

FSerialData CReaverBot_HanmuruDoll::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

CReaverBot_HanmuruDoll* CReaverBot_HanmuruDoll::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("ReaverBot_HanmuruDoll Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CReaverBot_HanmuruDoll* CReaverBot_HanmuruDoll::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Reaverbot_BReaverBot_HanmuruDollalfura Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CReaverBot_HanmuruDoll::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ReaverBot_HanmuruDoll Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CReaverBot_HanmuruDoll::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("ReaverBot_HanmuruDoll Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CReaverBot_HanmuruDoll::Free()
{
    SUPER::Free();
}

HRESULT CReaverBot_HanmuruDoll::Initialize_Component()
{
    return S_OK;
}

HRESULT CReaverBot_HanmuruDoll::Initialize_Component(FSerialData& InputData)
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
            break;
        case ECast(EComponentID::Collider):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pColliderComp = DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            m_pColliderComp->EnterToPhysics(0);
            break;
        }
    }

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

void CReaverBot_HanmuruDoll::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

}

void CReaverBot_HanmuruDoll::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CReaverBot_HanmuruDoll::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}

void CReaverBot_HanmuruDoll::Move_Update(const _float& fTimeDelta)
{
    if (m_ActionKey.IsAct(EActionKey::MoveForward))
        Transform().MoveForward(m_fMoveSpeed * fTimeDelta);
    else if (m_ActionKey.IsAct(EActionKey::MoveBackward))
        Transform().MoveForward(-m_fMoveSpeed * fTimeDelta);

    if (m_ActionKey.IsAct(EActionKey::TurnRight))
        Transform().TurnRight(m_fMoveSpeed * 0.2f * fTimeDelta);
    else if (m_ActionKey.IsAct(EActionKey::TurnLeft))
        Transform().TurnRight(-m_fMoveSpeed * 0.2f * fTimeDelta);

    if (m_ActionKey.IsAct(EActionKey::LookTarget)
        && nullptr != m_pTarget)
        Transform().Look_At_OnLand(m_pTarget->Transform().Get_PositionVector(), 5.f * fTimeDelta);
}

void CReaverBot_HanmuruDoll::Register_State()
{
    for (_uint i = 0; i < ECast(EActionKey::Size); i++)
        m_ActionKey.Add_Action(Cast<EActionKey>(i));

    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Walk, &ThisClass::ActState_Walk);
    m_State_Act.Add_Func(EState_Act::Ready_Smash, &ThisClass::ActState_Ready_Smash);
    m_State_Act.Add_Func(EState_Act::Smash, &ThisClass::ActState_Smash);
    m_State_Act.Add_Func(EState_Act::WalkAndSmash, &ThisClass::ActState_WalkAndSmash);
    m_State_Act.Add_Func(EState_Act::Damaged, &ThisClass::ActState_Damaged);
    m_State_Act.Add_Func(EState_Act::Dead, &ThisClass::ActState_Dead);
    m_State_Act.Set_State(EState_Act::Idle);

    m_State_AI.Add_Func(EState_AI::Idle, &ThisClass::AIState_Idle);
    m_State_AI.Add_Func(EState_AI::Chase, &ThisClass::AIState_Chase);
    m_State_AI.Add_Func(EState_AI::Smash, &ThisClass::AIState_Smash);
    m_State_AI.Add_Func(EState_AI::OverHit, &ThisClass::AIState_OverHit);
    m_State_AI.Add_Func(EState_AI::Dead, &ThisClass::AIState_Dead);
    m_State_AI.Set_State(EState_AI::Idle);
}

#pragma region 액션 스테이트
void CReaverBot_HanmuruDoll::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);

        // 공격
        if (m_ActionKey.IsAct(EActionKey::Smash))
            m_State_Act.Set_State(EState_Act::Smash);
        // 움직임
        if (m_ActionKey.IsAct(EActionKey::MoveForward) || m_ActionKey.IsAct(EActionKey::MoveBackward)
            || m_ActionKey.IsAct(EActionKey::TurnRight) || m_ActionKey.IsAct(EActionKey::TurnLeft)
            || m_ActionKey.IsAct(EActionKey::LookTarget))
            m_State_Act.Set_State(EState_Act::Walk);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_Walk(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(1, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);

        // 공격
        if (m_ActionKey.IsAct(EActionKey::Smash))
            m_State_Act.Set_State(EState_Act::Smash);
        // 움직임
        if (!m_ActionKey.IsAct(EActionKey::MoveForward) && !m_ActionKey.IsAct(EActionKey::MoveBackward)
            && !m_ActionKey.IsAct(EActionKey::TurnRight) && !m_ActionKey.IsAct(EActionKey::TurnLeft)
            && !m_ActionKey.IsAct(EActionKey::LookTarget))
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_Ready_Smash(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(4, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);

        // 공격
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Smash);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_Smash(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(5, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);

        // 원상태로 돌아감
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
        {
            if (m_fHP.Get_Percent() > 0.5f)
                Create_DamageCollision();
            else
                Create_DamageCollisionBig();
            m_State_Act.Set_State(EState_Act::Idle);
        }
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_WalkAndSmash(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(2, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Move_Update(fTimeDelta);

        // 원상태로 돌아감
        if (!m_ActionKey.IsAct(EActionKey::WalkAndSmash))
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_Damaged(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(3, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {

        // 원상태로 돌아감
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::ActState_Dead(const _float& fTimeDelta)
{

}
#pragma endregion


#pragma region 인공지능 스테이트
void CReaverBot_HanmuruDoll::AIState_Idle(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Readjust(0.1f);
    }

    if (m_State_AI.Can_Update())
    {
        m_pTarget = Find_Target();

        if (nullptr != m_pTarget)
        {
            _vector vLook = Transform().Get_LookNormalizedVector();
            _vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
                - Transform().Get_PositionVector(), 0.f));
            _float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
            if (fDot < 1.f - 0.001f)
                m_ActionKey.Act(EActionKey::LookTarget);
            else
            {
                if (m_fIdleTime.Increase(fTimeDelta))
                {
                    m_State_AI.Set_State(EState_AI::Chase);
                }
            }
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::AIState_Chase(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {

    }

    if (m_State_AI.Can_Update())
    {
        m_pTarget = Find_Target();

        if (nullptr != m_pTarget)
        {
            m_ActionKey.Act(EActionKey::LookTarget);
            m_ActionKey.Act(EActionKey::MoveForward);

            _float fDistance = XMVectorGetX(
                XMVector3Length((m_pTarget->Transform().Get_PositionVector() - Transform().Get_PositionVector())));
            _float fFindDistance = 4.f;
            if (m_fHP.Get_Percent() < 0.5f)
                fFindDistance = 10.f;

            if (fDistance < fFindDistance)
            {
                m_State_AI.Set_State(EState_AI::Smash);
            }
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::AIState_Smash(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        if (m_fHP.Get_Percent() >= 0.5f)
            m_fIdleTime.Readjust(4.f);
        else
            m_fIdleTime.Readjust(2.f);
        m_ActionKey.Act(EActionKey::Smash);
    }

    if (m_State_AI.Can_Update())
    {
        if (m_fIdleTime.Increase(fTimeDelta))
        {
            m_State_AI.Set_State(EState_AI::Idle);
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::AIState_OverHit(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        // 현재 필드에서 땅속으로 탈출한다.

        if (m_fIdleTime.Increase(fTimeDelta))
        {

        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_HanmuruDoll::AIState_Dead(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        // 현재 필드에서 땅속으로 탈출한다.

        if (m_fIdleTime.Increase(fTimeDelta))
        {

        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}
#pragma endregion


CCharacter_Common* CReaverBot_HanmuruDoll::Find_Target()
{
    auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), 30.f, COLLAYER_CHARACTER);
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

void CReaverBot_HanmuruDoll::Create_DamageCollision()
{
    _vector vPos = Transform().Get_PositionVector()
        + Transform().Get_LookNormalizedVector() * 3.f;
    _float3 vfPos = {};
    XMStoreFloat3(&vfPos, vPos);
    auto pDamageCollision = CDamageCollision::Create(vfPos);
    if (pDamageCollision == nullptr)
        return;

    GI()->Add_GameObject(pDamageCollision);
    pDamageCollision->Transform().Set_Scale(3.f, 3.f, 3.f);
    pDamageCollision->Set_LifeTime(0.01f);
    pDamageCollision->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
}

void CReaverBot_HanmuruDoll::Create_DamageCollisionBig()
{
    for (_uint i = 0; i < 3; i++)
    {
        _vector vPos = Transform().Get_PositionVector()
            + Transform().Get_LookNormalizedVector() * (i + (3.f * (i + 1)));
        _float3 vfPos = {};
        XMStoreFloat3(&vfPos, vPos);
        auto pDamageCollision = CDamageCollision::Create(vfPos);
        if (pDamageCollision == nullptr)
            return;

        GI()->Add_GameObject(pDamageCollision);
        pDamageCollision->Transform().Set_Scale(3.f + i * 2.f, 3.f + i * 2.f, 3.f + i * 2.f);
        pDamageCollision->Set_LifeTime(0.01f);
        pDamageCollision->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
    }
}
