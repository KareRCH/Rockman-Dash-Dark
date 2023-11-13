#include "System/Texture.h"

CTexture::CTexture(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

void CTexture::Free()
{
}
