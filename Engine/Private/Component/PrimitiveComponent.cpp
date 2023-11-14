#include "Component/PrimitiveComponent.h"

CPrimitiveComponent::CPrimitiveComponent(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_bClone(false)
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: m_pDevice(rhs.m_pDevice), m_pDeviceContext(rhs.m_pDeviceContext), m_bClone(true)
{
}

void CPrimitiveComponent::Free()
{
}


