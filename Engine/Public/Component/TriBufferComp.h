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
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CTriBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

protected:
	
};

END