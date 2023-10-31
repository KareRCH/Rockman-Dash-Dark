#include "Component/PrimitiveComponent.h"

CPrimitiveComponent::CPrimitiveComponent(ID3D11Device* pGraphicDev)
	: m_bClone(false)
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: m_bClone(true)
{
}

void CPrimitiveComponent::Free()
{
	
}


