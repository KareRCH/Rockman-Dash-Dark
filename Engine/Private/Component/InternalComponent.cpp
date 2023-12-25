#include "Component/InternalComponent.h"

CInternalComponent::CInternalComponent(const CInternalComponent& rhs)
	: Base(rhs)
{
}

void CInternalComponent::Free()
{
	SUPER::Free();
}
