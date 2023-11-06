#include "Component/PrimitiveComponent.h"

CPrimitiveComponent::CPrimitiveComponent(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_bClone(false)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: m_pDevice(rhs.m_pDevice), m_bClone(true)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

void CPrimitiveComponent::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}


