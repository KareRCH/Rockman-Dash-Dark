#pragma once

#include "Base.h"
#include "TextureMgr.h"

BEGIN(Engine)

/// <summary>
/// 텍스처 데이터 저장용 클래스
/// 추상클래스
/// </summary>
class CTexture abstract : public CBase
{
	DERIVED_CLASS(CBase, CTexture)

protected:
	explicit CTexture(ID3D11Device* pGraphicDev);
	virtual ~CTexture() = default;

public:
	virtual HRESULT		Initialize() PURE;

public:
	static CTexture*	Create(ID3D11Device* pGraphicDev);

private:
	virtual void		Free() PURE;

protected:
	virtual HRESULT		Insert_Texture(const _tchar* pFilePath, TEXTUREID eType, const _tchar* pStateKey = L"", const _range<_uint>& iCntRange = _range<_uint>(0U, 0U)) PURE;
	virtual void		Transfer_Texture() PURE;

protected:
	ID3D11Device*				m_pGraphicDev = nullptr;						// 장치

};

END