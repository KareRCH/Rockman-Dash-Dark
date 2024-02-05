#include "GameObject/InvisibleObject.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CInvisibleObject::CInvisibleObject()
{
    Set_Name(TEXT("InvisibleObject"));
}

CInvisibleObject::CInvisibleObject(const CInvisibleObject& rhs)
{
}

HRESULT CInvisibleObject::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(180.f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Map/Main/Door-Left.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
    m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
    m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
    m_pColliderComp->Bind_Collision(ECollisionType::OBB);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_WALL);

    return S_OK;
}

HRESULT CInvisibleObject::Initialize_Prototype(FSerialData& InputData)
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

HRESULT CInvisibleObject::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CInvisibleObject::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CInvisibleObject::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CInvisibleObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CInvisibleObject::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CInvisibleObject::Render()
{
    SUPER::Render();

    if (IsState(EGObjectState::Tool))
        m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

void CInvisibleObject::OnCreated()
{
    SUPER::OnCreated();

    TurnOn_State(EGObjectState::Cull);
}

void CInvisibleObject::BeginPlay()
{
    SUPER::BeginPlay();

    if (m_pColliderComp)
        m_pColliderComp->EnterToPhysics(0);
}

CInvisibleObject* CInvisibleObject::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("StaticObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CInvisibleObject* CInvisibleObject::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("StaticObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInvisibleObject::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("StaticObject Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CInvisibleObject::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("StaticObject Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CInvisibleObject::Free()
{
    SUPER::Free();
}

FSerialData CInvisibleObject::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

FSerialData CInvisibleObject::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

HRESULT CInvisibleObject::Initialize_Component()
{


    return S_OK;
}

HRESULT CInvisibleObject::Initialize_Component(FSerialData& InputData)
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
            Set_CullRadius(XMVectorGetX(XMVector3Length(m_pColliderComp->Transform().Get_ScaleVector() * 6.f)));
            break;
        }
    }

    return S_OK;
}

void CInvisibleObject::OnCollision(CGameObject* pDst, const FContact* const pContact)
{

}

void CInvisibleObject::OnCollisionEntered(CGameObject* pDst, const FContact* const pContact)
{

}

void CInvisibleObject::OnCollisionExited(CGameObject* pDst)
{

}
