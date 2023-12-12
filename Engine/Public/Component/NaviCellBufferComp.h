#pragma once
#include "VIBufferComp.h"

BEGIN(Engine)

class ENGINE_DLL CNaviCellBufferComp final : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CNaviCellBufferComp)
protected:
	explicit CNaviCellBufferComp() = default;
	explicit CNaviCellBufferComp(const CNaviCellBufferComp& rhs);
	virtual ~CNaviCellBufferComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg) override { return S_OK; }
	virtual HRESULT	Initialize_Prototype(const _float3* pPoints);
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CNaviCellBufferComp* Create(const _float3* pPoints);
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void		Free() override;

};

END
