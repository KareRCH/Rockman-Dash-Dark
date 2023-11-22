#include "Component/AnimationComponent.h"

CAnimationComponent::CAnimationComponent()
{
}

CAnimationComponent::CAnimationComponent(const CAnimationComponent& rhs)
	: Base(rhs)
{
}

HRESULT CAnimationComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CAnimationComponent::Initialize(void* Arg)
{
	return S_OK;
}

CAnimationComponent* CAnimationComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("CAnimationComponent Create Failed");

		return nullptr;
	}

	return pInstance;
}

CComponent* CAnimationComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("CAnimationComponent Create Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CAnimationComponent::Free()
{

}
