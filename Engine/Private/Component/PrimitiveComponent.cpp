#include "Component/PrimitiveComponent.h"

CPrimitiveComponent::CPrimitiveComponent(ID3D11Device* pDevice)
	: m_pDevice(pDevice), m_bClone(false)
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: m_pDevice(rhs.m_pDevice), m_bClone(true)
{
}

void CPrimitiveComponent::Free()
{
	
}


