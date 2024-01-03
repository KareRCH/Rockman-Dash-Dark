#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class CInstancingBufferComp abstract : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CInstancingBufferComp)
protected:
	explicit CInstancingBufferComp() = default;
	explicit CInstancingBufferComp(const CInstancingBufferComp& rhs);
	virtual ~CInstancingBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	virtual CComponent* Clone(void* Arg = nullptr) override;
};

END