#include "Component/Component.h"

CComponent::CComponent()
	: m_bClone(true)
{
}

CComponent::CComponent(const CComponent& rhs)
	: m_bClone(false)
{
}