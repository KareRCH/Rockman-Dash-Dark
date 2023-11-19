#include "Component/MaterialComponent.h"

CMaterialComponent::CMaterialComponent(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
}

CMaterialComponent::CMaterialComponent(const CMaterialComponent& rhs)
    : Base(rhs)
{
}

HRESULT CMaterialComponent::Initialize()
{
    return S_OK;
}

void CMaterialComponent::Priority_Tick(const _float& fTimeDelta)
{
}

_int CMaterialComponent::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CMaterialComponent::Late_Tick(const _float& fTimeDelta)
{
}

void CMaterialComponent::Render()
{
}

CMaterialComponent* CMaterialComponent::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("CMaterialComponent Create Failed");

		return nullptr;
	}

	return pInstance;
}

CComponent* CMaterialComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("CMaterialComponent Create Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CMaterialComponent::Free()
{
	SUPER::Free();
}
