#include "Component/PlaneModelComp.h"

CPlaneModelComp::CPlaneModelComp(const CPlaneModelComp& rhs)
    : Base(rhs)
{

}

HRESULT CPlaneModelComp::Initialize_Prototype(void* Arg)
{
    __super::Initialize_Prototype(Arg);

	return S_OK;
}

HRESULT CPlaneModelComp::Initialize(void* Arg)
{
	return S_OK;
}

void CPlaneModelComp::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CPlaneModelComp::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CPlaneModelComp::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CPlaneModelComp::Render()
{
	return S_OK;
}

CPlaneModelComp* CPlaneModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("PlaneModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CPlaneModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("PlaneModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CPlaneModelComp::Free()
{
    SUPER::Free();
}
