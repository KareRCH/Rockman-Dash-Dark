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
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CTriBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

protected:
	
};

END