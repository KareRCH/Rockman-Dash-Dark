#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// �ִϸ��̼��� ������ �� �ִ� ������Ʈ
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