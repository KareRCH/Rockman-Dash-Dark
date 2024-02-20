#pragma once

#include "Base.h"
#include "Engine_Define.h"

#include <mutex>

BEGIN(Engine)

class CTexture;

/// <summary>
/// �ؽ�ó�� �����ϴ� �Ŵ���.
/// �ؽ�ó�� ó�� ������ ���ҽ��� ��θ� ��������,
/// ��ΰ����� Ű��, ���� �̸��� Ȯ���ڸ� �� ������ �̸��� ����.
/// ������� ���� �߰������� ���� ������.
/// �׷��Ͽ��� ���͸��� ���ִ� �ؽ�ó ��θ� ���� �Ŵ����� �����Ͽ� �ؽ�ó�� ���� �� �ִ����Ѵ�.
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
	map<wstring, CTexture*>		m_mapTextures;				// �ؽ�ó ���� ����, �н� �̸��� Ű��. ���� �ؽ�ó�� ���� �������� ����. ���⼭�� �з��� ���� ����.

};

END