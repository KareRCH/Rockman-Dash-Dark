#pragma once

#include "Component/InstancingBufferComp.h"


BEGIN(Engine)

class ENGINE_DLL CParticlePointBufferComp final : public CInstancingBufferComp
{
	DERIVED_CLASS(CInstancingBufferComp, CParticlePointBufferComp)

protected:
	explicit CParticlePointBufferComp();
	explicit CParticlePointBufferComp(const CParticlePointBufferComp& rhs);
	virtual ~CParticlePointBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize_Prototype(_uint iNumInstance);
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CParticlePointBufferComp* Create();
	static CParticlePointBufferComp* Create(_uint iNumInstance);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void Free() override;

public:
	virtual HRESULT Create_VIBuffers(_uint iNumInstance);

};

END