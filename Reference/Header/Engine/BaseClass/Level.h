#pragma once

#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
	DERIVED_CLASS(CBase, CLevel)
protected:
	explicit CLevel() = default;
	explicit CLevel(const CLevel& rhs) = delete;
	virtual ~CLevel() = default;

public:
	virtual HRESULT		Initialize() PURE;
	virtual void		Tick(const _float& fTimeDelta) PURE;
	virtual HRESULT		Render() PURE;

protected:
	virtual void		Free() PURE;

};

END