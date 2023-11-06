#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CTriBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CTriBufferComp)
protected:
	explicit CTriBufferComp(const DX11DEVICE_T tDevice);
	explicit CTriBufferComp(const CTriBufferComp& rhs);
	virtual ~CTriBufferComp() = default;

public:
	virtual HRESULT Initialize();
	virtual void	PriorityTick();
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick();
	virtual void	Render();

public:
	static CTriBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CPrimitiveComponent* Clone(void* Arg);

protected:
	virtual void	Free();

protected:
	
};

END