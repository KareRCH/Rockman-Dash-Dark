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
	ID3D11Texture2D*			Find_Texture2D(const wstring& strTextureKey);
	ID3D11ShaderResourceView*	Find_SRV(const wstring& strTextureKey);
	HRESULT						Load_Texture(const wstring& strFilePath, const _bool bPermanent);
	HRESULT						Load_Texture(const wstring& strFilePath, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent);
	HRESULT						Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, const wstring& strTextureKey);

public:
	//GETSET_1(mutex, m_mapMutex, Mutex, GET_PTR)

private:
	wstring						m_strMainPath = { L"" };
	map<wstring, CTexture*>		m_mapTextures;				// �ؽ�ó ���� ����, �н� �̸��� Ű��. ���� �ؽ�ó�� ���� �������� ����. ���⼭�� �з��� ���� ����.
	//mutex						m_mapMutex;					// �ؽ�ó ��ü ������ ����ȭ

};

END