#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent()
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

_int CTransformComponent::Update(const _float& fTimeDelta)
{
	SUPER::Update(fTimeDelta);

	return 0;
}

void CTransformComponent::LateUpdate()
{

}

void CTransformComponent::Render()
{

}

void CTransformComponent::Create(ID3D11Device* m_pDevice)
{
	
}

void CTransformComponent::Free()
{
	SUPER::Free();
}
