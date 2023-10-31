#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
{
}

CTransformComponent::CTransformComponent(const CTransformComponent& rhs)
	: Base(rhs)
{
}

CTransformComponent::~CTransformComponent()
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

void CTransformComponent::Create(ID3D11Device* m_pDevice)
{
	
}

void CTransformComponent::Free()
{
	SUPER::Free();
}
