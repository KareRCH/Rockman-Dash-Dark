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
	// ���� SRV�� ��� �Լ� [��]
	ID3D11ShaderResourceView*	Get_ShaderResourseView(const _uint iIndex);
	// �ؽ�ó �迭�� �޾� �ش� �迭�� SRV�� �Ѱ��ִ� �Լ� [������]
	HRESULT						Get_ShaderResourceViews(ID3D11ShaderResourceView** Arr, const _uint iNumTextures);

public:
	// �ؽ�ó�� ���ε��Ѵ�. ���ÿ� �ؽ�ó �Ŵ����� ��ϵ� �Ѵ�. [��]
	HRESULT Bind_Texture(const wstring& strFilePath, const _uint iNumTextures = 1);
	// �ؽ�ó �����ϱ� [��]
	HRESULT Unbind_Texture();

public:
	// ����Ʈ�� �ؽ�ó ���ε� [��]
	HRESULT Bind_SRVToEffect(class CEffectComponent* pEffect, const _char* pTextureSymbolName, const _uint iIndex);

private:
	// �ؽ�ó �Ŵ����� �����ϴ� ���� �Լ� [��]
	HRESULT Link_TextureManager();


private:
	// S���ξ�� �ɺ��̶�� ��
	using SShaderResourceViews = vector<ComPtr<ID3D11ShaderResourceView>>;
	class CTextureMgr*		m_pTextureMgr = { nullptr };	// ���� �ؽ�ó ���ҽ� ������ ���δ�.

	_uint					m_iNumTextures = 0;
	SShaderResourceViews	m_vecSRVs = { nullptr };		// ���� ������ �Ǵ� �ؽ�ó�� �ּ��̴�.
};

END