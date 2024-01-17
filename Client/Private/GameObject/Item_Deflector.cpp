#include "GameObject/Item_Deflector.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"

CItem_Deflector::CItem_Deflector()
{
    Set_Name(TEXT("Item_Deflector"));
}

CItem_Deflector::CItem_Deflector(const CItem_Deflector& rhs)
{
}

HRESULT CItem_Deflector::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(90.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Buster/Buster.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_ITEM);
    m_pColliderComp->Set_CollisionMask(COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize_Prototype(FSerialData& InputData)
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

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CItem_Deflector::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CItem_Deflector::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_pColliderComp->Tick(fTimeDelta);

}

void CItem_Deflector::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CItem_Deflector::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

CItem_Deflector* CItem_Deflector::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CItem_Deflector* CItem_Deflector::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Deflector::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CItem_Deflector::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CItem_Deflector::Free()
{
    SUPER::Free();
}

HRESULT CItem_Deflector::Initialize_Component()
{
    

    return S_OK;
}

HRESULT CItem_Deflector::Initialize_Component(FSerialData& InputData)
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

    return S_OK;
}

FSerialData CItem_Deflector::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

FSerialData CItem_Deflector::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

void CItem_Deflector::OnCollision(CGameObject* pDst, const FContact* pContact)
{
}

void CItem_Deflector::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
}

void CItem_Deflector::OnCollisionExited(CGameObject* pDst)
{
}
