#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

CTransformComponent::CTransformComponent(const CTransformComponent& rhs)
	: Base(rhs)
{
}

HRESULT CTransformComponent::Initialize()
{

	return S_OK;
}

void CTransformComponent::PriorityTick()
{
}

_int CTransformComponent::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	return 0;
}

void CTransformComponent::LateTick()
{

}

void CTransformComponent::Render()
{

}

CTransformComponent* CTransformComponent::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("CGraphicDev Create Failed");
		
		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CTransformComponent::Clone(void* Arg)
{
	return new ThisClass(*this);
}

void CTransformComponent::Free()
{
	SUPER::Free();
}


