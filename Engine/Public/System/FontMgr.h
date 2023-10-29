#pragma once

#include "Base.h"

#include "System/MyFont.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CFontMgr : public CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CFontMgr)
private:
	explicit CFontMgr();
	virtual ~CFontMgr();

private:
	virtual void	Free();

public:
	HRESULT		Ready_Font(ID3D11Device* pGraphicDev,
							const _tchar* pFontTag,
							const _tchar* pFontType,
							const _uint& iWidth,
							const _uint& iHeight,
							const _uint& iWeight);

	void		Render_Font(const _tchar* pFontTag,
							const _tchar* pString,
							const _vec2* pPos,
							D3DCOLOR Color);

private:
	CMyFont* Find_Font(const _tchar* pFontTag);

private:
	_unmap<const _tchar*, CMyFont*>			m_mapFont;


};

END_NAME