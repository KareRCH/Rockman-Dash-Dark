#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CTriBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CTriBufferComp)
protected:
	explicit CTriBufferComp() = default;
	explicit CTriBufferComp(const CTriBufferComp& rhs);
	virtual ~CTriBufferComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

	virtual HRESULT Render_Buffer();

public:
	static CTriBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

protected:
	
};

END