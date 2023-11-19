#include "Component/InternalComponent.h"

CInternalComponent::CInternalComponent()
{
}

CInternalComponent::CInternalComponent(const CInternalComponent& rhs)
	: Base(rhs)
{
}

void CInternalComponent::Free()
{
	SUPER::Free();
}
