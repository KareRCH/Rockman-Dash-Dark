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
	ID3D11ShaderResourceView* Get_SRV(_uint iIndex);

private:
	const static _uint	g_iNumTextures = AI_TEXTURE_TYPE_MAX;
	array<class CTextureComponent*, g_iNumTextures>	m_vecTextureComp = { nullptr };
	

};

END