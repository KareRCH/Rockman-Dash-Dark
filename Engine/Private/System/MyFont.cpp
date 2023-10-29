#include "System/MyFont.h"

CMyFont::CMyFont(ID3D11Device* pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CMyFont::~CMyFont()
{
}

HRESULT CMyFont::Ready_Font(const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight)
{

	return S_OK;
}

void CMyFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DCOLOR Color)
{
	RECT	rc{ LONG(pPos->x), LONG(pPos->y) };

}

CMyFont* CMyFont::Create(ID3D11Device* pGraphicDev, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	ThisClass* pInstance = new ThisClass(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
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
