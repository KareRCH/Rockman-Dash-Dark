#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class CBoxBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CBoxBufferComp)
protected:
	explicit CBoxBufferComp() = default;
	explicit CBoxBufferComp(const CBoxBufferComp& rhs);
	virtual ~CBoxBufferComp() = default;

public:
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CBoxBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();
};

END