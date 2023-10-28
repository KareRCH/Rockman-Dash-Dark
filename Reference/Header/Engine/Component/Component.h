#pragma once

#include "Base.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
	DERIVED_CLASS(CBase, CComponent)
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Update() PURE;
	virtual void	LateUpdate() PURE;
	virtual void	Render() PURE;
};

END_NAME