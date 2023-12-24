#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CRectBufferComp final : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CRectBufferComp)
protected:
	explicit CRectBufferComp() = default;
	explicit CRectBufferComp(const CRectBufferComp& rhs);
	virtual ~CRectBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CRectBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;
};

END