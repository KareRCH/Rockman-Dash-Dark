#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// 머터리얼을 관리할 수 있는 컴포넌트
/// </summary>
class ENGINE_DLL CMaterialComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CMaterialComponent)
protected:
	explicit CMaterialComponent(const DX11DEVICE_T tDevice);
	explicit CMaterialComponent(const CMaterialComponent& rhs);
	virtual ~CMaterialComponent() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CMaterialComponent* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg);

protected:
	virtual void					Free();
};

END