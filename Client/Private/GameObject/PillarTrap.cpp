#include "GameObject/PillarTrap.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CPillarTrap::CPillarTrap()
{
    Set_Name(TEXT("PillarTrap"));
    Set_RenderGroup(ERenderGroup::NonBlend);
}

CPillarTrap::CPillarTrap(const CPillarTrap& rhs)
{

}

HRESULT CPillarTrap::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Map/Object/Trap/PillarTrap.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
    m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
    m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
    m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_OBJECT);

    return S_OK;
}

HRESULT CPillarTrap::Initialize_Prototype(FSerialData& InputData)
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

    return S_OK;
}

HRESULT CPillarTrap::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CPillarTrap::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CPillarTrap::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CPillarTrap::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_pColliderComp->Tick(fTimeDelta);
}

void CPillarTrap::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();
    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CPillarTrap::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

void CPillarTrap::BeginPlay()
{
    SUPER::BeginPlay();

    if (m_pColliderComp)
        m_pColliderComp->EnterToPhysics(0);
}

CPillarTrap* CPillarTrap::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CPillarTrap* CPillarTrap::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPillarTrap::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CPillarTrap::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CPillarTrap::Free()
{
    SUPER::Free();
}

FSerialData CPillarTrap::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("TrapType", ECast(m_eTrapType));
    Data.Add_Member("Range", m_fRadiusRange);

    return Data;
}

FSerialData CPillarTrap::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("TrapType", ECast(m_eTrapType));
    Data.Add_Member("Range", m_fRadiusRange);

    return Data;
}

HRESULT CPillarTrap::Initialize_Component()
{
    return S_OK;
}

HRESULT CPillarTrap::Initialize_Component(FSerialData& InputData)
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
            m_pColliderComp->Set_CollisionKinematic();
            break;
        }
    }

    return S_OK;
}

void CPillarTrap::OnCollision(CGameObject* pDst, const FContact* const pContact)
{
}

void CPillarTrap::OnCollisionEntered(CGameObject* pDst, const FContact* const pContact)
{
}

void CPillarTrap::OnCollisionExited(CGameObject* pDst)
{
}
