#include "Component/AnimationComponent.h"

CAnimationComponent::CAnimationComponent()
{
}

CAnimationComponent::CAnimationComponent(const CAnimationComponent& rhs)
	: Base(rhs)
{
}

HRESULT CAnimationComponent::Initialize()
{
	return S_OK;
}

void CAnimationComponent::Priority_Tick(const _float& fTimeDelta)
{
}

_int CAnimationComponent::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CAnimationComponent::Late_Tick(const _float& fTimeDelta)
{
}

void CAnimationComponent::Render()
{
}

CAnimationComponent* CAnimationComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
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
	SUPER::Free();
}
