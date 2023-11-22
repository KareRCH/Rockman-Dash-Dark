#pragma once

#include "SceneComponent.h"


BEGIN(Engine)

/// <summary>
/// 텍스처를 저장하는 컴포넌트
/// </summary>
class ENGINE_DLL CTextureComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CTextureComponent)
protected:
	explicit CTextureComponent() = default;
	explicit CTextureComponent(const CTextureComponent& rhs);
	virtual ~CTextureComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CTextureComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();
};

END