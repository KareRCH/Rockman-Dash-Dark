#pragma once

#include "Base.h"

BEGIN(Engine)

class CMyFont : public CBase
{
	DERIVED_CLASS(CBase, CMyFont)
private:
	explicit CMyFont(ID3D11Device* pGraphicDev);
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
	static CMyFont* Create(ID3D11Device* pGraphicDev,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

private:
	virtual void		Free();

private:
	ID3D11Device* m_pGraphicDev;
	/*ID3D11Sprite*				m_pSprite;
	LPD3DXFONT					m_pFont;*/

};

END