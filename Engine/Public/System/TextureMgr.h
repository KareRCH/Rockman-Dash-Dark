#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include <mutex>

BEGIN(Engine)

class CTexture;

/// <summary>
/// 텍스처를 관리하는 매니저
/// </summary>
class CTextureMgr final : public CBase
{
	DERIVED_CLASS(CBase, CTextureMgr)

private:
	explicit CTextureMgr();
	virtual ~CTextureMgr() = default;

public:
	HRESULT			Initialize(ID3D11Device* pGraphicDev);

public:
	static CTextureMgr* Create(ID3D11Device* pGraphicDev);

private:
	virtual void	Free();

public:
	HRESULT			Load_Texture(TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey, const _range<_uint>& iCntRange = _range<_uint>(0U, 0U));
	HRESULT			Insert_Texture(const _tchar* pFilePath, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey = L"", const _range<_uint>& iCntRange = _range<_uint>(0U, 0U));
	HRESULT			Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey = L"");

public:
	GETSET_1(mutex, m_mapMutex, Mutex, GET_PTR)

private:
	ID3D11Device*					m_pGraphicDev = nullptr;
	_unmap<wstring, CTexture*>		m_mapTexture;
	mutex							m_mapMutex;				// 텍스처 객체 제조시 동기화

};

END