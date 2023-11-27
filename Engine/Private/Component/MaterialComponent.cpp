#include "Component/MaterialComponent.h"

CMaterialComponent::CMaterialComponent()
{
}

CMaterialComponent::CMaterialComponent(const CMaterialComponent& rhs)
    : Base(rhs)
{
}

HRESULT CMaterialComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CMaterialComponent::Initialize(void* Arg)
{
    return S_OK;
}

CMaterialComponent* CMaterialComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CMaterialComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CMaterialComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CMaterialComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CMaterialComponent::Free()
{

}
