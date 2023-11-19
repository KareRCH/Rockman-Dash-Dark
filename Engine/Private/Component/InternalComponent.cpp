#include "Component/InternalComponent.h"

CInternalComponent::CInternalComponent(const DX11DEVICE_T tDevice)
	: Base(tDevice)
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
