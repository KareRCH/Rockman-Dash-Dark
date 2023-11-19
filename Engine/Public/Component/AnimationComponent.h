#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// 애니메이션을 관리할 수 있는 컴포넌트
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent(const DX11DEVICE_T tDevice);
	explicit CAnimationComponent(const CAnimationComponent& rhs);
	virtual ~CAnimationComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CAnimationComponent* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg);

protected:
	virtual void					Free();
};

END