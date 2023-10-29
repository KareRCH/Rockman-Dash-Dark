#pragma once

#include "Base.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CMyFont : public CBase
{
	DERIVED_CLASS(CBase, CMyFont)
private:
	explicit CMyFont(ID3D11Device* pGraphicDev);
	virtual ~CMyFont();

public:
	HRESULT			Ready_Font(const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

	void			Render_Font(const _tchar* pString,
		const _vec2* pPos,
		D3DCOLOR Color);

private:
	ID3D11Device*				m_pGraphicDev;
	/*LPD3DXSPRITE				m_pSprite;
	LPD3DXFONT					m_pFont;*/

public:
	static CMyFont* Create(ID3D11Device* pGraphicDev,
		const _tchar* pFontType,
		const _uint& iWidth,
		const _uint& iHeight,
		const _uint& iWeight);

private:
	virtual void		Free();
};

END_NAME