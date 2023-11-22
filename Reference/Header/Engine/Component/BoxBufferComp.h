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
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CBoxBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;
};

END