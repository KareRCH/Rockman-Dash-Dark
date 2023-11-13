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
		Safe_Release(pTextureView);
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
		Safe_Release(pTextureView);

		pTexture = _pTexture;
		pTextureView = _pTextureView;

		// 둘중 하나가 nullptr이면 로드되지 않았음.
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
	_bool						bLoaded = false;			// 로드된 텍스처 체크용
	_bool						bPermanent = false;			// 영구 유지 텍스처 설정, 전역으로 사용되는 용도
};

/// <summary>
/// 텍스처 데이터 저장용 클래스
/// 추상클래스
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
	ComPtr<ID3D11Device>				m_pDevice = { nullptr };					// 장치
	ComPtr<ID3D11DeviceContext>			m_pDeviceContext = { nullptr };				// 장치 컨텍스트

};

END