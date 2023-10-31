#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CVIBufferComp abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CVIBufferComp)
protected:
	explicit CVIBufferComp(ID3D11Device* pGraphicDev);
	explicit CVIBufferComp(const CVIBufferComp& rhs);
	virtual ~CVIBufferComp() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() PURE;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) PURE;

public:
	virtual CPrimitiveComponent*	Clone() PURE;

protected:
	virtual void	Free();
};

END