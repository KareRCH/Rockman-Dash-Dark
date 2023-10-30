#include "System/FontMgr.h"

CFontMgr::CFontMgr()
{
}

HRESULT CFontMgr::Initialize()
{
	return S_OK;
}

CFontMgr* CFontMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("FontMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CFontMgr::Free()
{
	for (auto iter = m_mapFont.begin(); iter != m_mapFont.end(); ++iter)
	{
		Safe_Release((*iter).second);
	}
	m_mapFont.clear();
}

HRESULT CFontMgr::Create_Font(ID3D11Device* pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	CMyFont* pMyFont = nullptr;

	pMyFont = Find_Font(pFontTag);

	if (nullptr != pMyFont)
		return E_FAIL;

	// 폰트 객체 생성하고 관리를 위해 map에 넣는다.
	pMyFont = CMyFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	NULL_CHECK_RETURN(pMyFont, E_FAIL);

	m_mapFont.emplace(pFontTag, pMyFont);

	return S_OK;
}

void CFontMgr::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DCOLOR Color)
{
	CMyFont* pMyFont = Find_Font(pFontTag);
	NULL_CHECK(pMyFont);

	pMyFont->Render_Font(pString, pPos, Color);
}

CMyFont* CFontMgr::Find_Font(const _tchar* pFontTag)
{
	auto iter = m_mapFont.find(pFontTag);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

