#pragma once

#include "System/Texture.h"

BEGIN(Engine)

/// <summary>
/// 키값을 통해 텍스처 분류를 해놓은 텍스처 클래스
/// 
/// </summary>
class CMultiStateTexture final : public CTexture
{
	DERIVED_CLASS(CTexture, CMultiStateTexture)

private:
	explicit CMultiStateTexture(const DX11DEVICE_T tDevice);
	virtual ~CMultiStateTexture() = default;

public:
	virtual HRESULT				Initialize();

public:
	static CMultiStateTexture*	Create(const DX11DEVICE_T tDevice);

private:
	virtual void				Free();

public:
	// 공간 할당만 해놓는 함수
	virtual	HRESULT	Reserve(const string& strStateKey, _bool bPermanent);
	virtual HRESULT Insert_Texture(const string& strFilePath, const string& strStateKey, const _bool bPermanent) override;
	virtual void	Transfer_Texture() override {};
	virtual void	Transfer_Texture(ID3D11ShaderResourceView* pTexture, const string& strStateKey);

protected:
	using multi_tex_map = _unmap<string, FTextureData*>;
	multi_tex_map			m_mapTexture;				// 텍스처 정보

};

END