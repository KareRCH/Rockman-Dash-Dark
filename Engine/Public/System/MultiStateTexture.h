#pragma once

#include "System/Texture.h"

BEGIN(Engine)

/// <summary>
/// Ű���� ���� �ؽ�ó �з��� �س��� �ؽ�ó Ŭ����
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
	// ���� �Ҵ縸 �س��� �Լ�
	virtual	HRESULT						Reserve(const wstring& strTextureKey, _bool bPermanent);
	virtual HRESULT						Insert_Texture(const wstring& strFilePath, const wstring& strTextureKey, const _bool bPermanent) override;
	virtual ID3D11ShaderResourceView*	Get_Texture(const wstring& strTextureKey) override;
	virtual void						Transfer_Texture() override {};
	virtual void						Transfer_Texture(ID3D11ShaderResourceView* pTexture, const wstring& strTextureKey);

protected:
	using multi_tex_map = _unmap<wstring, FTextureData*>;
	multi_tex_map			m_mapTexture;				// �ؽ�ó ����

	

};

END