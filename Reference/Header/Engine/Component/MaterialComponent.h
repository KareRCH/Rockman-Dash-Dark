#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IMaterialComponent.h"

BEGIN(Engine)

/// <summary>
/// 머터리얼을 관리할 수 있는 컴포넌트
/// 모델로부터 머터리얼 정보를 로드하여 정해진 머터리얼의 텍스처 정보를 여기에 저장한다.
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
	// 모델로부터 머터리얼의 정보를 얻어온다. 이때 경로를 사용해서 실제 텍스처 정보도 얻어온다.
	HRESULT Load_Materials(EModelGroupIndex eGroupIndex, const wstring& strModelKey, _uint iIndex);
	ID3D11ShaderResourceView* Get_SRV(_uint iIndex);
	HRESULT Bind_TextureToEffect(class CEffectComponent* pEffectComp, const _char* pConstantName, _uint iTextureIndex);

private:
	const static _uint	g_iNumTextures = AI_TEXTURE_TYPE_MAX;
	array<class CTextureComponent*, g_iNumTextures>	m_arrTextureComps = { nullptr };
	

};

END