#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent(ID3D11Device* pDevice)
	: Base(pDevice)
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

_int CTransformComponent::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	return 0;
}

void CTransformComponent::LateTick()
{

}

void CTransformComponent::Render(ID3D11DeviceContext* pDeviceContext)
{

}

CTransformComponent* CTransformComponent::Create(ID3D11Device* m_pDevice)
{
	ThisClass* pInstance = new ThisClass(m_pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("CGraphicDev Create Failed");
		
		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CTransformComponent::Clone()
{
	return new ThisClass(*this);
}

void CTransformComponent::Free()
{
	SUPER::Free();
}


