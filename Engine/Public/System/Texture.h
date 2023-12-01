#pragma once

#include "TextureMgr.h"

BEGIN(Engine)

class FTextureData final
{
	THIS_CLASS(FTextureData)
private:
	explicit FTextureData() = default;
	explicit FTextureData(const FTextureData& rhs) = default;
public:
	~FTextureData() = default;

public:
	static	FTextureData* Create(ID3D11Texture2D* const pTexture, ID3D11ShaderResourceView* const pTextureView,
		const _bool bPermanent)
	{
		ThisClass* pInstance = new ThisClass();

		if (FAILED(pInstance->Load(pTexture, pTextureView)))
		{
			Safe_Delete(pInstance);

			MSG_BOX("Texture Create Failed");
			return nullptr;
		}

		pInstance->bPermanent = bPermanent;

		return pInstance;
	}

	// ���� ������, ���� �����ÿ��� ����� ��.
	void	Free()
	{
		UnLoad();
		delete this;
	}

	// �޸𸮸� ������ �� ���.
	void	UnLoad()
	{
		Safe_Release(pTexture);
		Safe_Release(pSRV);
		bLoaded = false;
	}

	// �ε�� ���, �ε�� ������ �� �ε����� �ʴ´�.
	HRESULT	Load(ID3D11Texture2D* _pTexture, ID3D11ShaderResourceView* _pTextureView)
	{
		if (bLoaded)
		{
			// ���� ������ �ڵ����� �����Ѵ�.
			Safe_Release(_pTexture);
			Safe_Release(_pTextureView);

			return E_FAIL;
		}

		Safe_Release(pTexture);
		Safe_Release(pSRV);

		pTexture = _pTexture;
		pSRV = _pTextureView;

		// ���� �ϳ��� nullptr�̸� �ε���� �ʾ���.
		if (pTexture && pSRV)
			bLoaded = true;

		return S_OK;
	}

	void Set_Permanent(const _bool value) { bPermanent = value; }
	const _bool& Is_Loaded() const { return bLoaded; }
	ID3D11Texture2D* const Get_Texture() { return pTexture; }
	ID3D11ShaderResourceView* const Get_SRV() { return pSRV; }

private:
	ID3D11Texture2D*			pTexture = nullptr;			// CPU�� �ؽ�ó
	ID3D11ShaderResourceView*	pSRV = nullptr;				// ���̴� ���÷�
	_bool						bLoaded = false;			// �ε�� �ؽ�ó üũ��
	_bool						bPermanent = false;			// ���� ���� �ؽ�ó ����, �������� ���Ǵ� �뵵
};

/// <summary>
/// �ؽ�ó ������ ����� Ŭ����
/// �߻�Ŭ����
/// </summary>
class CTexture : public CBase
{
	DERIVED_CLASS(CBase, CTexture)

protected:
	explicit CTexture(const DX11DEVICE_T tDevice);
	explicit CTexture(const CTexture& rhs) = delete;
	virtual ~CTexture() = default;

public:
	virtual HRESULT		Initialize();

public:
	static	CTexture* Create(const DX11DEVICE_T tDevice);
	
protected:
	virtual void		Free();

protected:
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };					// ��ġ
	ComPtr<ID3D11DeviceContext>			m_pDeviceContext = { nullptr };				// ��ġ ���ؽ�Ʈ

public:
	// �޸𸮸� ������ �� ���.
	void	UnLoad();
	HRESULT	Load(ID3D11Texture2D* _pTexture, ID3D11ShaderResourceView* _pSRV);
	
public:
	virtual HRESULT						Insert_Texture(const wstring& strFilePath, const wstring& strName, const _bool bPermanent);

public:
	void								Set_Permanent(const _bool value) { m_bPermanent = value; }
	const _bool&						Is_Loaded() const { return m_bLoaded; }
	ID3D11Texture2D* const				Get_Texture() { return m_pTexture.Get(); }
	ID3D11ShaderResourceView* const		Get_SRV() { return m_pSRV.Get(); }

private:
	wstring								m_strName = { L"" };		// �ؽ�ó �̸�	(Ȯ���� ���Ž� �̸�)
	ComPtr<ID3D11Texture2D>				m_pTexture = { nullptr };	// CPU�� �ؽ�ó
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };		// ���̴� ���÷�
	_bool								m_bLoaded = false;			// �ε�� �ؽ�ó üũ��
	_bool								m_bPermanent = false;		// ���� ���� �ؽ�ó ����, �������� ���Ǵ� �뵵

};

END