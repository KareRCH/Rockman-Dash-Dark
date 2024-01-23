#include "GameObject/ReaverBot_Balfura.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Door_Common.h"

CReaverBot_Balfura::CReaverBot_Balfura()
{
    Set_Name(TEXT("ReaverBot_Balfura"));
    Set_RenderGroup(ERenderGroup::NonBlend);
    m_fHP = FGauge(8.f, true);
    Register_State();
}

CReaverBot_Balfura::CReaverBot_Balfura(const CReaverBot_Balfura& rhs)
    : Base(rhs)
{
    Register_State();
}

HRESULT CReaverBot_Balfura::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.32f, 0.32f, 0.32f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Balfura/Balfura.amodel");
    m_pModelComp->Set_Animation(0, 1.f, true);

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
        | COLLAYER_ATTACKER | COLLAYER_OBJECT);

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize_Prototype(FSerialData& InputData)
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

HRESULT CReaverBot_Balfura::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;
    

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CReaverBot_Balfura::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Balfura::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    //Input_ActionKey();
    m_State_AI.Get_StateFunc()(this, fTimeDelta);
    m_State_Act.Get_StateFunc()(this, fTimeDelta);
    m_ActionKey.Reset();

    m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Balfura::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Balfura::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

FSerialData CReaverBot_Balfura::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

FSerialData CReaverBot_Balfura::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CReaverBot_Balfura::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CReaverBot_Balfura::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Balfura::Free()
{
    SUPER::Free();
}

HRESULT CReaverBot_Balfura::Initialize_Component()
{
    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize_Component(FSerialData& InputData)
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

void CReaverBot_Balfura::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

    CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
    if (pDoor)
    {
        _float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
        _vector vSimNormal = {};
        vSimNormal = XMLoadFloat3(&vNormal);
        Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
        if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
            m_bIsOnGround = true;
    }
}

void CReaverBot_Balfura::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CReaverBot_Balfura::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}

void CReaverBot_Balfura::Register_State()
{
    for (_uint i = 0; i < ECast(EActionKey::Size); i++)
        m_ActionKey.Add_Action(Cast<EActionKey>(i));

    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Dead, &ThisClass::ActState_Dead);
    m_State_Act.Set_State(EState_Act::Idle);

    m_State_AI.Add_Func(EState_AI::Idle, &ThisClass::AIState_Idle);
    m_State_AI.Add_Func(EState_AI::Look, &ThisClass::AIState_Look);
    m_State_AI.Add_Func(EState_AI::SlowChase, &ThisClass::AIState_SlowChase);
    m_State_AI.Add_Func(EState_AI::Charge, &ThisClass::AIState_Charge);
    m_State_AI.Add_Func(EState_AI::Prowl, &ThisClass::AIState_Prowl);
    m_State_AI.Set_State(EState_AI::Idle);
}

void CReaverBot_Balfura::Update_Move(const _float& fTimeDelta)
{
    if (m_ActionKey.IsAct(EActionKey::FastMove))
        m_fSpeed = 10.f;
    else if (m_ActionKey.IsAct(EActionKey::SlowMove))
        m_fSpeed = 1.5f;
    else
        m_fSpeed = 5.f;

    if (m_ActionKey.IsAct(EActionKey::MoveForward))
        Transform().MoveForward(m_fSpeed * fTimeDelta);
    else if (m_ActionKey.IsAct(EActionKey::MoveBackward))
        Transform().MoveForward(-m_fSpeed * fTimeDelta);

    if (m_ActionKey.IsAct(EActionKey::TurnRight))
        Transform().TurnRight(m_fTurnSpeed * fTimeDelta);
    else if (m_ActionKey.IsAct(EActionKey::TurnLeft))
        Transform().TurnRight(-m_fTurnSpeed * fTimeDelta);

    if (m_ActionKey.IsAct(EActionKey::LookHorizon))
        Transform().Look_At_OnLand(Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector(), m_fTurnSpeed * fTimeDelta);
    else if (m_ActionKey.IsAct(EActionKey::LookTarget))
    {
        if (nullptr != m_pTarget)
        {
            _matrix TargetMatrix = m_pTarget->ColliderComp()->Calculate_TransformMatrixFromParent();
            Transform().Look_At(TargetMatrix.r[3], m_fTurnSpeed * fTimeDelta);
        }
    }
}

void CReaverBot_Balfura::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        Update_Move(fTimeDelta);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::ActState_Dead(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {

    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::AIState_Idle(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        m_ActionKey.Act(EActionKey::MoveForward);
        m_ActionKey.Act(EActionKey::SlowMove);

        m_pTarget = Find_Target(10.f);

        if (nullptr != m_pTarget)
        {
            _vector vLook = Transform().Get_LookNormalizedVector();
            _vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
                - Transform().Get_PositionVector(), 0.f));
            _float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
            if (fDot < 1.f - 0.001f)
            {
                m_ActionKey.Act(EActionKey::LookTarget);
            }

            if (m_fIdleTime.Increase(fTimeDelta))
            {
                m_State_AI.Set_State(EState_AI::Look);
            }
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::AIState_Look(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        m_pTarget = Find_Target(6.f);

        if (nullptr != m_pTarget)
        {
            _vector vLook = Transform().Get_LookNormalizedVector();
            _vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
                - Transform().Get_PositionVector(), 0.f));
            _float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
            if (fDot < 1.f - 0.001f)
            {
                m_ActionKey.Act(EActionKey::LookTarget);
            }

            if (m_fIdleTime.Increase(fTimeDelta))
            {
                m_State_AI.Set_State(EState_AI::Charge);
            }
        }

        if (m_fIdleTime.Increase(fTimeDelta))
        {
            m_State_AI.Set_State(EState_AI::Idle);
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::AIState_SlowChase(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        m_ActionKey.Act(EActionKey::MoveForward);
        m_ActionKey.Act(EActionKey::SlowMove);
        m_ActionKey.Act(EActionKey::LookTarget);

        m_pTarget = Find_Target(10.f);
        if (nullptr != m_pTarget)
        {
            _vector vLook = Transform().Get_LookNormalizedVector();
            _vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
                - Transform().Get_PositionVector(), 0.f));
            _float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
            if (fDot < 1.f - 0.001f)
            {
                m_ActionKey.Act(EActionKey::LookTarget);
            }

            if (m_fIdleTime.Increase(fTimeDelta))
            {
                m_State_AI.Set_State(EState_AI::Look);
            }
        }

        if (m_fIdleTime.Increase(fTimeDelta))
        {
            m_State_AI.Set_State(EState_AI::Idle);
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::AIState_Charge(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {
        m_ActionKey.Act(EActionKey::MoveForward);
        m_ActionKey.Act(EActionKey::FastMove);
        m_ActionKey.Act(EActionKey::LookTarget);

        m_pTarget = Find_Target(10.f);
        if (nullptr != m_pTarget)
        {
            _vector vLook = Transform().Get_LookNormalizedVector();
            _vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
                - Transform().Get_PositionVector(), 0.f));
            _float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
            if (fDot < 1.f - 0.001f)
            {
                m_ActionKey.Act(EActionKey::LookTarget);
            }

            if (m_fIdleTime.Increase(fTimeDelta))
            {
                m_State_AI.Set_State(EState_AI::Idle);
            }
        }

        if (m_fIdleTime.Increase(fTimeDelta))
        {
            m_State_AI.Set_State(EState_AI::Idle);
        }
    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

void CReaverBot_Balfura::AIState_Prowl(const _float& fTimeDelta)
{
    if (m_State_AI.IsState_Entered())
    {
        m_fIdleTime.Reset();
    }

    if (m_State_AI.Can_Update())
    {

    }

    if (m_State_AI.IsState_Exit())
    {

    }
}

CCharacter_Common* CReaverBot_Balfura::Find_Target(_float fSearchDistance)
{
    auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), fSearchDistance, COLLAYER_CHARACTER);
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
