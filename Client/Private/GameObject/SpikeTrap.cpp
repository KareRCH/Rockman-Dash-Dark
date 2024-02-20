#include "GameObject/SpikeTrap.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CSpikeTrap::CSpikeTrap()
{
    Set_Name(TEXT("SpikeTrap"));
}

CSpikeTrap::CSpikeTrap(const CSpikeTrap& rhs)
{

}

HRESULT CSpikeTrap::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Trap/SpikeTrap.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
    m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
    m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
    m_pColliderComp->Bind_Collision(ECollisionType::OBB);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_OBJECT);

    return S_OK;
}

HRESULT CSpikeTrap::Initialize_Prototype(FSerialData& InputData)
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
            break;
        }
    }

    _float fTemp = {};
    InputData.Get_Data("IdleTime", fTemp);
    m_fIdleTime.Readjust(fTemp);

    return S_OK;
}

HRESULT CSpikeTrap::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CSpikeTrap::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CSpikeTrap::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CSpikeTrap::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_State_Act.Get_StateFunc()(this, fTimeDelta);

    m_pColliderComp->Tick(fTimeDelta);
}

void CSpikeTrap::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();
    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CSpikeTrap::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

void CSpikeTrap::BeginPlay()
{
    SUPER::BeginPlay();

    Register_State();

    /*if (m_pColliderComp)
        m_pColliderComp->EnterToPhysics(0);*/
}

CSpikeTrap* CSpikeTrap::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CSpikeTrap Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CSpikeTrap* CSpikeTrap::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CSpikeTrap Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSpikeTrap::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CSpikeTrap Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CSpikeTrap::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("CSpikeTrap Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CSpikeTrap::Free()
{
    SUPER::Free();
}

FSerialData CSpikeTrap::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("IdleTime", m_fIdleTime.fMax);

    return Data;
}

FSerialData CSpikeTrap::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("IdleTime", m_fIdleTime.fMax);

    return Data;
}

HRESULT CSpikeTrap::Initialize_Component()
{
    return S_OK;
}

HRESULT CSpikeTrap::Initialize_Component(FSerialData& InputData)
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
                m_pModelComp = DynCast<CCommonModelComp*>(m_pGI->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pModelComp->Set_Animation(0, 1.f, true);
            break;
        case ECast(EComponentID::Collider):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pColliderComp = DynCast<CColliderComponent*>(m_pGI->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            m_pColliderComp->Set_CollisionKinematic();
            break;
        }
    }

    _float fTemp = {};
    InputData.Get_Data("IdleTime", fTemp);
    m_fIdleTime.Readjust(fTemp);

    return S_OK;
}

void CSpikeTrap::OnCollision(CGameObject* pDst, const FContact* const pContact)
{
}

void CSpikeTrap::OnCollisionEntered(CGameObject* pDst, const FContact* const pContact)
{
}

void CSpikeTrap::OnCollisionExited(CGameObject* pDst)
{
}

void CSpikeTrap::Register_State()
{
    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Attack, &ThisClass::ActState_Attack);
    m_State_Act.Add_Func(EState_Act::AttackEnd, &ThisClass::ActState_AttackEnd);
    m_State_Act.Set_State(EState_Act::Idle);
}

void CSpikeTrap::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, false);
        m_fIdleTime.Reset();
    }

    if (m_State_Act.Can_Update())
    {
        // 원래 바라보던 위치로 바라보면 Idle로
        if (m_fIdleTime.Increase(fTimeDelta))
            m_State_Act.Set_State(EState_Act::Attack);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CSpikeTrap::ActState_Attack(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(1, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        if (m_pModelComp->AnimationComp()->IsAnimation_Range(5.f, 6.f))
        {
            if (m_pColliderComp)
                m_pColliderComp->EnterToPhysics(0);
        }

        // 원래 바라보던 위치로 바라보면 Idle로
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::AttackEnd);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CSpikeTrap::ActState_AttackEnd(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(1, 1.f, false, true);
    }

    if (m_State_Act.Can_Update())
    {
        if (m_pModelComp->AnimationComp()->IsAnimation_Range(5.f, 6.f))
        {
            if (m_pColliderComp)
                m_pColliderComp->ExitFromPhysics(0);
        }

        // 원래 바라보던 위치로 바라보면 Idle로
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

