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
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

	void	Render_Buffer();

public:
	static	CBoxBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;
};

END