#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include <mutex>

BEGIN(Engine)

class CTexture;

/// <summary>
/// �ؽ�ó�� �����ϴ� �Ŵ���
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
	mutex							m_mapMutex;				// �ؽ�ó ��ü ������ ����ȭ

};

END