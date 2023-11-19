#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent()
	: m_qtOrientation(_float4(0.f, 0.f, 0.f, 1.f)), m_vScale(1.f, 1.f, 1.f)
	, m_matTransform()
{
	XMStoreFloat4x4(&m_matTransform, XMMatrixIdentity());
}

CTransformComponent::CTransformComponent(const CTransformComponent& rhs)
	: Base(rhs)
{
}

HRESULT CTransformComponent::Initialize()
{

	return S_OK;
}

CTransformComponent* CTransformComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("CGraphicDev Create Failed");
		
		return nullptr;
	}

	return pInstance;
}

CComponent* CTransformComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TransformComponent Copy Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTransformComponent::Free()
{
	SUPER::Free();
}


