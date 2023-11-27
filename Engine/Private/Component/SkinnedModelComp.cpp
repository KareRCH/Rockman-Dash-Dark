#include "Component/SkinnedModelComp.h"

CSkinnedModelComp::CSkinnedModelComp(const CSkinnedModelComp& rhs)
    : Base(rhs)
{
}

HRESULT CSkinnedModelComp::Initialize_Prototype(void* Arg)
{


    return S_OK;
}

HRESULT CSkinnedModelComp::Initialize(void* Arg)
{


    return S_OK;
}

void CSkinnedModelComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CSkinnedModelComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CSkinnedModelComp::Late_Tick(const _float& fTimeDelta)
{
}

void CSkinnedModelComp::Render()
{
}

CSkinnedModelComp* CSkinnedModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CSkinnedModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CSkinnedModelComp::Free()
{
    SUPER::Free();

}
