#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include <mutex>

BEGIN(Engine)

class CTexture;

/// <summary>
/// 텍스처를 관리하는 매니저.
/// 텍스처는 처음 설정된 리소스의 경로를 기준으로,
/// 경로값으로 키를, 파일 이름에 확장자를 뺀 값으로 이름을 가짐.
/// 경로폴더 또한 추가적으로 값을 가진다.
/// 그로하여금 머터리얼에 들어가있는 텍스처 경로를 토대로 매니저에 접근하여 텍스처를 얻을 수 있더럭한다.
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
	ComPtr<ID3D11Device>		m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = { nullptr };

public:
	HRESULT						IsExists_SRV(const wstring& strTextureKey);
	ID3D11ShaderResourceView*	Find_SRV(const wstring& strTextureKey) { return Find_SRV(strTextureKey, 0); }
	ID3D11ShaderResourceView*	Find_SRV(const wstring& strTextureKey, const _uint iIndex);
	HRESULT						Reference_SRVs(const wstring& strTextureKey, vector<ComPtr<ID3D11ShaderResourceView>>& RefSRVs);
	HRESULT						Load_Texture(const wstring& strFilePath, const _uint iNumTextures, const _bool bPermanent, _bool bUseMainPath);

public:
	GETSET_1(wstring, m_strMainPath, MainPath, GET_C_REF)

private:
	wstring						m_strMainPath = { L"" };
	map<wstring, CTexture*>		m_mapTextures;				// 텍스처 정보 저장, 패스 이름이 키값. 이제 텍스처는 단일 정보만을 가짐. 여기서는 분류를 하지 않음.

};

END