#pragma once

#include "Component/InternalComponent.h"


BEGIN(Engine)

/// <summary>
/// �ؽ�ó�� �����ϴ� ������Ʈ.
/// �ؽ�ó�� �ε��� �� �ؽ�ó �Ŵ����� �ִٸ�
/// �ؽ�ó�� ������ش�.
/// ���� �ؽ�ó�� �ҷ����� ��� ��ü�� ���� �ؽ�ó �Ŵ����� ���������� ����
/// ������Ʈ ���ο��� ����� ������ �����̰�, �Ŵ����� ���������� ��� ��Ű�� ������� �� �����̴�.
/// 
/// ���ο� ����صΰ� ���Ǵ� ������Ʈ�̴�.
/// �ܺο��� �ؽ�ó�� ���� ������ ��� �ʹٸ� ������ ��ü���� �Լ��� �����Ͽ� �򵵷� �����Ѵ�.
/// </summary>
class ENGINE_DLL CTextureComponent : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CTextureComponent)

public:
	struct FInitTextureComp
	{
		wstring strFilePath;
	};

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
	ID3D11ShaderResourceView* Get_ShaderResourseView() { return m_pSRV.Get(); }

public:
	// �ؽ�ó�� ���ε��Ѵ�. ���ÿ� �ؽ�ó �Ŵ����� ��ϵ� �Ѵ�.
	HRESULT Bind_Texture(const wstring& strFilePath);
	// �̹� ��ϵ� �ؽ�ó�� ã�� �����ϴ� �Լ�, ������ �� �Ⱦ���? [Deprecated]
	HRESULT Bind_TextureFromManager(const wstring& strFilePath);
	// �ؽ�ó ���� ����
	HRESULT Unbind_Texture();

public:
	HRESULT Bind_SRV(class CEffectComponent* pEffect, const _char* pTextureName);

private:
	// �ؽ�ó �Ŵ����� �����ϴ� ���� �Լ�
	HRESULT Link_TextureManager();


private:
	class CTextureMgr*					m_pTextureMgr = { nullptr };	// ���� �ؽ�ó ���ҽ� ������ ���δ�.
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };			// ���� ������ �Ǵ� �ؽ�ó�� �ּ��̴�.
};

END