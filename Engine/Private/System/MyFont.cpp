#include "System/MyFont.h"

CMyFont::CMyFont(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CMyFont::Initialize(const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{

	return S_OK;
}

void CMyFont::Render(const _tchar* pString, const _float2* pPos, D3DCOLOR Color)
{
	RECT	rc{ LONG(pPos->x), LONG(pPos->y) };

	/*m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rc, DT_NOCLIP, Color);

	m_pSprite->End();*/
}

CMyFont* CMyFont::Create(const DX11DEVICE_T tDevice, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(pFontType, iWidth, iHeight, iWeight)))
	{
		Safe_Release(pInstance);
		MSG_BOX("폰트 문제야");
		return nullptr;
	}

	return pInstance;
}

void CMyFont::Free()
{
}
