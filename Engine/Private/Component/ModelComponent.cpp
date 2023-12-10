#include "Component/ModelComponent.h"

CModelComponent::CModelComponent(const CModelComponent& rhs)
	: Base(rhs)
{
}

HRESULT CModelComponent::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModelComponent::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

	return S_OK;
}

void CModelComponent::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);
}

void CModelComponent::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);
}

void CModelComponent::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);
}

void CModelComponent::Free()
{
	SUPER::Free();


}


