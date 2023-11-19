#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class CBoxBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CBoxBufferComp)
protected:
	explicit CBoxBufferComp(const DX11DEVICE_T tDevice);
	explicit CBoxBufferComp(const CBoxBufferComp& rhs);
	virtual ~CBoxBufferComp() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CBoxBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();
};

END