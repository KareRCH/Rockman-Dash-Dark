#include "System/FontMgr.h"

CFontMgr::CFontMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pContext(tDevice.pDeviceContext)
{
}

HRESULT CFontMgr::Initialize(const wstring& strMainPath)
{
	m_strMainPath = strMainPath;

	return S_OK;
}

CFontMgr* CFontMgr::Create(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(strMainPath)))
	{
		MSG_BOX("FontMgr Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CFontMgr::Free()
{
	for (auto& FontPair : m_mapFonts)
		Safe_Release(FontPair.second);
	m_mapFonts.clear();
}

HRESULT CFontMgr::Add_Font(const wstring& strFontTag, const wstring& strFontFilePath)
{
	auto iter = m_mapFonts.find(strFontTag);
	if (iter != m_mapFonts.end())
		return E_FAIL;

	CCustomFont* pFont = CCustomFont::Create({ m_pDevice, m_pContext }, m_strMainPath + strFontFilePath);
	if (nullptr == pFont)
		return E_FAIL;

	m_mapFonts.emplace(strFontTag, pFont);

	return S_OK;
}

HRESULT CFontMgr::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _fvector vColor, _float fScale, _float2 vOrigin, _float fRotation)
{
	CCustomFont* pFont = Find_Font(strFontTag);
	if (nullptr == pFont)
		return E_FAIL;

	return pFont->Render(strText, vPosition, vColor, fScale, vOrigin, fRotation);
}

CCustomFont* CFontMgr::Find_Font(const wstring& strFontName)
{
	auto iter = m_mapFonts.find(strFontName);
	if (iter == m_mapFonts.end())
		return nullptr;

	return iter->second;
}

