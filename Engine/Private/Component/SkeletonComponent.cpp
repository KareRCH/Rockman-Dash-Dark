#include "Component/SkeletonComponent.h"

CSkeletonComponent::CSkeletonComponent(const DX11DEVICE_T tDevice)
    : Base(tDevice)
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

CSkeletonComponent* CSkeletonComponent::Create(const DX11DEVICE_T tDevice)
{
    ThisClass* pInstance = new ThisClass(tDevice);

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
