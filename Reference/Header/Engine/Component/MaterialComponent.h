#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IMaterialComponent.h"

BEGIN(Engine)

/// <summary>
/// ���͸����� ������ �� �ִ� ������Ʈ
/// �𵨷κ��� ���͸��� ������ �ε��Ͽ� ������ ���͸����� �ؽ�ó ������ ���⿡ �����Ѵ�.
/// </summary>
class ENGINE_DLL CMaterialComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CMaterialComponent)
protected:
	explicit CMaterialComponent();
	explicit CMaterialComponent(const CMaterialComponent& rhs);
	virtual ~CMaterialComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CMaterialComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;


public:
	// �𵨷κ��� ���͸����� ������ ���´�. �̶� ��θ� ����ؼ� ���� �ؽ�ó ������ ���´�.
	HRESULT Load_Materials(EModelGroupIndex eGroupIndex, const wstring& strModelKey, _uint iIndex);
	ID3D11ShaderResourceView* Get_SRV(_uint iIndex);
	HRESULT Bind_TextureToEffect(class CEffectComponent* pEffectComp, const _char* pConstantName, _uint iTextureIndex);

private:
	const static _uint	g_iNumTextures = AI_TEXTURE_TYPE_MAX;
	array<class CTextureComponent*, g_iNumTextures>	m_arrTextureComps = { nullptr };
	

};

END