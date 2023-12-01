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

	// 완전 삭제용, 완전 해제시에만 사용할 것.
	void	Free()
	{
		UnLoad();
		delete this;
	}

	// 메모리만 해제할 때 사용.
	void	UnLoad()
	{
		Safe_Release(pTexture);
		Safe_Release(pSRV);
		bLoaded = false;
	}

	// 로드시 사용, 로드된 상태일 시 로드하지 않는다.
	HRESULT	Load(ID3D11Texture2D* _pTexture, ID3D11ShaderResourceView* _pTextureView)
	{
		if (bLoaded)
		{
			// 들어온 정보를 자동으로 해제한다.
			Safe_Release(_pTexture);
			Safe_Release(_pTextureView);

			return E_FAIL;
		}

		Safe_Release(pTexture);
		Safe_Release(pSRV);

		pTexture = _pTexture;
		pSRV = _pTextureView;

		// 둘중 하나가 nullptr이면 로드되지 않았음.
		if (pTexture && pSRV)
			bLoaded = true;

		return S_OK;
	}

	void Set_Permanent(const _bool value) { bPermanent = value; }
	const _bool& Is_Loaded() const { return bLoaded; }
	ID3D11Texture2D* const Get_Texture() { return pTexture; }
	ID3D11ShaderResourceView* const Get_SRV() { return pSRV; }

private:
	ID3D11Texture2D*			pTexture = nullptr;			// CPU용 텍스처
	ID3D11ShaderResourceView*	pSRV = nullptr;				// 셰이더 샘플러
	_bool						bLoaded = false;			// 로드된 텍스처 체크용
	_bool						bPermanent = false;			// 영구 유지 텍스처 설정, 전역으로 사용되는 용도
};

/// <summary>
/// 텍스처 데이터 저장용 클래스
/// 추상클래스
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
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };					// 장치
	ComPtr<ID3D11DeviceContext>			m_pDeviceContext = { nullptr };				// 장치 컨텍스트

public:
	// 메모리만 해제할 때 사용.
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
	wstring								m_strName = { L"" };		// 텍스처 이름	(확장자 제거시 이름)
	ComPtr<ID3D11Texture2D>				m_pTexture = { nullptr };	// CPU용 텍스처
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };		// 셰이더 샘플러
	_bool								m_bLoaded = false;			// 로드된 텍스처 체크용
	_bool								m_bPermanent = false;		// 영구 유지 텍스처 설정, 전역으로 사용되는 용도

};

END