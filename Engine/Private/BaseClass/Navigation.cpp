#include "BaseClass/Navigation.h"

CNavigation::CNavigation()
{
    Set_Name(L"Navigation");
}

CNavigation::CNavigation(const CNavigation& rhs)
    : Base(rhs)
{
}

HRESULT CNavigation::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

#ifdef _DEBUG
    TurnOn_State(EGObjectState::Render);
#endif

    return S_OK;
}

HRESULT CNavigation::Initialize_Prototype(const _float3 vPos)
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

#ifdef _DEBUG
    TurnOn_State(EGObjectState::Render);
#endif

    return S_OK;
}

HRESULT CNavigation::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize(Arg)))
        return E_FAIL;


    return S_OK;
}

HRESULT CNavigation::Initialize(const _float3 vPos)
{

    return S_OK;
}

void CNavigation::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CNavigation::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CNavigation::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    Transform().Set_Position(PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One));
}

HRESULT CNavigation::Render()
{
    SUPER::Render();


    return S_OK;
}

CNavigation* CNavigation::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CNavigation* CNavigation::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CNavigation::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CNavigation::Free()
{
    SUPER::Free();
}

HRESULT CNavigation::Initialize_Component()
{


    return S_OK;
}
