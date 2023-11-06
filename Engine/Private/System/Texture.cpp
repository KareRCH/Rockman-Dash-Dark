#include "System/Texture.h"

CTexture::CTexture(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

void CTexture::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
}
