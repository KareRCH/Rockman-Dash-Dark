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
		Safe_Release(pTextureView);
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
		Safe_Release(pTextureView);

		pTexture = _pTexture;
		pTextureView = _pTextureView;

		// ���� �ϳ��� nullptr�̸� �ε���� �ʾ���.
		if (pTexture && pTextureView)
			bLoaded = true;

		return S_OK;
	}

	void Set_Permanent(const _bool value) { bPermanent = value; }
	const _bool& Is_Loaded() const { return bLoaded; }
	ID3D11ShaderResourceView* const Get_Texture() { return pTextureView; }

private:
	ID3D11Texture2D*			pTexture = nullptr;
	ID3D11ShaderResourceView*	pTextureView = nullptr;
	_bool						bLoaded = false;			// �ε�� �ؽ�ó üũ��
	_bool						bPermanent = false;			// ���� ���� �ؽ�ó ����, �������� ���Ǵ� �뵵
};

/// <summary>
/// �ؽ�ó ������ ����� Ŭ����
/// �߻�Ŭ����
/// </summary>
class CTexture abstract : public CBase
{
	DERIVED_CLASS(CBase, CTexture)

protected:
	explicit CTexture(const DX11DEVICE_T tDevice);
	explicit CTexture(const CTexture& rhs) = delete;
	virtual ~CTexture() = default;

public:
	virtual HRESULT		Initialize() PURE;

protected:
	virtual void		Free();

public:
	virtual	HRESULT						Reserve(const wstring& strTextureKey, _bool bPermanent) PURE;
	virtual HRESULT						Insert_Texture(const wstring& strFilePath, const wstring& strTextureKey, const _bool bPermanent) PURE;
	virtual ID3D11ShaderResourceView*	Get_Texture(const wstring& strTextureKey) PURE;
	virtual void						Transfer_Texture() PURE;

protected:
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };					// ��ġ
	ComPtr<ID3D11DeviceContext>			m_pDeviceContext = { nullptr };				// ��ġ ���ؽ�Ʈ

};

END