#pragma once

#include "Component/InternalComponent.h"


BEGIN(Engine)

/// <summary>
/// �ؽ�ó�� �����ϴ� ������Ʈ.
/// ���� ���� �׽�Ʈ�� ���ؼ� ���̰� �ִ�.
/// </summary>
class ENGINE_DLL CTextureComponent : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CTextureComponent)
protected:
	explicit CTextureComponent() = default;
	explicit CTextureComponent(const CTextureComponent& rhs);
	virtual ~CTextureComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CTextureComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

public:
	HRESULT Bind_TextureFromManager(const wstring& strFilePath);

private:
	ComPtr<ID3D11Texture2D>				m_pTexture = { nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };
};

END