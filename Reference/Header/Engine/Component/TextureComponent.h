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
	explicit CTextureComponent(const DX11DEVICE_T tDevice);
	explicit CTextureComponent(const CTextureComponent& rhs);
	virtual ~CTextureComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CTextureComponent* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();
};

END