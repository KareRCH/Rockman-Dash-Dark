#pragma once

#include "Base.h"

#include "System/MyFont.h"

BEGIN(Engine)

/// <summary>
/// 폰트 관리 클래스
/// </summary>
class ENGINE_DLL_DBG CFontMgr final : public CBase
{
	DERIVED_CLASS(CBase, CFontMgr)
private:
	explicit CFontMgr(const DX11DEVICE_T tDevice);
	virtual ~CFontMgr() = default;

public:
	HRESULT				Initialize();

public:
	static CFontMgr*	Create(const DX11DEVICE_T tDevice);

private:
	virtual void		Free();


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;

public:
	HRESULT		Create_Font(const _tchar* pFontTag,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

	void		Render_Font(const _tchar* pFontTag,
							const _tchar* pString,
							const _float2* pPos,
							D3DCOLOR Color);

private:
	CMyFont*	Find_Font(const _tchar* pFontTag);

private:
	_unmap<const _tchar*, CMyFont*>			m_mapFont;

};

END