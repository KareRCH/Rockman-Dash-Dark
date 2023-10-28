#pragma once

#include "Base.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
	DERIVED_CLASS(CBase, CGameObject)
protected:
	explicit CGameObject();
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

};

END_NAME