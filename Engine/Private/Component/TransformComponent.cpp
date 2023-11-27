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

HRESULT CTransformComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CTransformComponent::Initialize(void* Arg)
{
	return S_OK;
}

CTransformComponent* CTransformComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CGraphicDev Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CTransformComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("TransformComponent Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTransformComponent::Free()
{
	
}


