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
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem_Deflector::Initialize_Prototype(const _float3 vPos)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    Transform().Set_Position(vPos);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(const _float3 vPos)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    Transform().Set_Position(vPos);

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

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CItem_Deflector::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    m_pColliderComp->Render();
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

CItem_Deflector* CItem_Deflector::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
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

void CItem_Deflector::Free()
{
    SUPER::Free();
}

HRESULT CItem_Deflector::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

    m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(90.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Buster/Buster.amodel");

    if (nullptr == m_pColliderComp)
        return E_FAIL;
    m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_ITEM);
    m_pColliderComp->Set_CollisionMask(COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);

    return S_OK;
}

FSerialData CItem_Deflector::SerializeData()
{
    return FSerialData();
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
