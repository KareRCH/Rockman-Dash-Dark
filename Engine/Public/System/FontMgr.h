#pragma once

#include "Base.h"

#include "System/CustomFont.h"

BEGIN(Engine)

/// <summary>
/// 폰트 관리 클래스
/// </summary>
class ENGINE_DLL_DBG CFontMgr final : public CBase
{
	DERIVED_CLASS(CBase, CFontMgr)
private:
	explicit CFontMgr(const DX11DEVICE_T tDevice);
	explicit CFontMgr(const CFontMgr& rhs) = delete;
	virtual ~CFontMgr() = default;

public:
	HRESULT				Initialize(const wstring& strMainPath);

public:
	static CFontMgr*	Create(const DX11DEVICE_T tDevice, const wstring& strMainPath);

private:
	virtual void		Free();


public:
	HRESULT			Add_Font(const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT			Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, 
								_fvector vColor, _float fScale, _float2 vOrigin, _float fRotation);
private:
	CCustomFont*	Find_Font(const wstring& strFontName);


private:
	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pContext = { nullptr };


private:
	wstring							m_strMainPath = TEXT("");
	_unmap<wstring, CCustomFont*>	m_mapFonts;

};

END