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
	explicit CTextureMgr(const DX11DEVICE_T tDevice);
	explicit CTextureMgr(const CTextureMgr& rhs) = delete;
	virtual ~CTextureMgr() = default;

public:
	HRESULT			Initialize(const wstring& strMainPath);

public:
	static CTextureMgr* Create(const DX11DEVICE_T tDevice, const wstring& strMainPath);

private:
	virtual void	Free();

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pDeviceContext = nullptr;

public:
	ID3D11ShaderResourceView*	Get_Texture(const wstring& strGroupKey, const wstring& strTextureKey);
	HRESULT						Load_Texture(const wstring& strFilePath, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent);
	HRESULT						Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, const wstring& strGroupKey, const wstring& strTextureKey);

public:
	//GETSET_1(mutex, m_mapMutex, Mutex, GET_PTR)

private:
	wstring							m_strMainPath = L"";
	_unmap<wstring, CTexture*>		m_mapTextureGroup;
	//mutex							m_mapMutex;				// 텍스처 객체 제조시 동기화

};

END