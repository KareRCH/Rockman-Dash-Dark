#include "Component/SceneComponent.h"

CSceneComponent::CSceneComponent(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
	, m_vPosition(_float3()), m_vRotation(_float3()), m_vScale(_float3()), m_qtOrientation(_float4())
	, m_matTransform(XMMatrixIdentity())
{
	
}

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
{
}

CSceneComponent::~CSceneComponent()
{
}

_int CSceneComponent::Tick(const _float& fTimeDelta)
{
	// ���⿡ �ڵ� �Լ� �߰�

	return 0;
}

void CSceneComponent::Free()
{
	SUPER::Free();
}
