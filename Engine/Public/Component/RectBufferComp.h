#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CRectBufferComp final : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CRectBufferComp)
protected:
	explicit CRectBufferComp(const DX11DEVICE_T tDevice);
	explicit CRectBufferComp(const CRectBufferComp& rhs);
	virtual ~CRectBufferComp() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CRectBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();
};

END