#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

/// <summary>
/// 다른 컴포넌트에 의해 내부적으로 관리되는 컴포넌트.
/// 오브젝트의 요소로써 동작하는 컴포넌트가 바로 내부 컴포넌트라고 한다.
/// </summary>
class CInternalComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CInternalComponent)
protected:
	explicit CInternalComponent(const DX11DEVICE_T tDevice);
	explicit CInternalComponent(const CInternalComponent& rhs);
	virtual ~CInternalComponent() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg) PURE;

protected:
	virtual void Free();
};

END