#pragma once

#include "Component/InstancingBufferComp.h"

BEGIN(Engine)

class CParticleRectBufferComp : public CInstancingBufferComp
{
	DERIVED_CLASS(CInstancingBufferComp, CParticleRectBufferComp)
protected:
	explicit CParticleRectBufferComp() = default;
	explicit CParticleRectBufferComp(const CParticleRectBufferComp& rhs);
	virtual ~CParticleRectBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CParticleRectBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;
};

END
