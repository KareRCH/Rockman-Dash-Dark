#include "Component/SkeletonComponent.h"

CSkeletonComponent::CSkeletonComponent()
{
}

CSkeletonComponent::CSkeletonComponent(const CSkeletonComponent& rhs)
    : Base(rhs)
{
}

HRESULT CSkeletonComponent::Initialize()
{
    return S_OK;
}

void CSkeletonComponent::Priority_Tick(const _float& fTimeDelta)
{
}

_int CSkeletonComponent::Tick(const _float& fTimeDelta)
{
    return 0;
}

CSkeletonComponent* CSkeletonComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("CSkeletonComponent Create Failed");

        return nullptr;
    }

    return pInstance;
}

CComponent* CSkeletonComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("CSkeletonComponent Create Failed");

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CSkeletonComponent::Free()
{
    SUPER::Free();
}
