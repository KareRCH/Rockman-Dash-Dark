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
	explicit CTextureMgr(const DX11DEVICE_T tDevice);
	virtual ~CTextureMgr() = default;

public:
	HRESULT			Initialize();

public:
	static CTextureMgr* Create(const DX11DEVICE_T tDevice);

private:
	virtual void	Free();

public:
	HRESULT			Load_Texture(TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey, const _range<_uint>& iCntRange = _range<_uint>(0U, 0U));
	HRESULT			Insert_Texture(const _tchar* pFilePath, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey = L"", const _range<_uint>& iCntRange = _range<_uint>(0U, 0U));
	HRESULT			Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey = L"");

public:
	GETSET_1(mutex, m_mapMutex, Mutex, GET_PTR)

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDeviceContext = nullptr;
	_unmap<wstring, CTexture*>		m_mapTexture;
	mutex							m_mapMutex;				// �ؽ�ó ��ü ������ ����ȭ

};

END