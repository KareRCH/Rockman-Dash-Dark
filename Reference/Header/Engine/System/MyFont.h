#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL_DBG CMyFont final : public CBase
{
	DERIVED_CLASS(CBase, CMyFont)
private:
	explicit CMyFont(const DX11DEVICE_T tDevice);
	virtual ~CMyFont() = default;

public:
	HRESULT			Initialize(const _tchar* pFontType,
								const _uint& iWidth,
								const _uint& iHeight,
								const _uint& iWeight);

	void			Render(const _tchar* pString,
							const _float2* pPos,
							D3DCOLOR Color);

public:
	static CMyFont* Create(const DX11DEVICE_T tDevice,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

private:
	virtual void		Free();

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
	/*ID3D11Sprite*				m_pSprite;
	LPD3DXFONT					m_pFont;*/

};

END