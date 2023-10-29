#include "Component/SceneComponent.h"

CSceneComponent::CSceneComponent()
	: m_vPosition(_vec3()), m_vRotation(_vec3()), m_vScale(_vec3()), m_qtOrientation(_vec4())
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

_int CSceneComponent::Update(const _float& fTimeDelta)
{
	// ���⿡ �ڵ� �Լ� �߰�

	return 0;
}

void CSceneComponent::Free()
{
	SUPER::Free();
}
