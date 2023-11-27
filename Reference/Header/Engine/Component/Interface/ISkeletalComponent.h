#pragma once

#include "Engine_Macro.h"

BEGIN(Engine)

class CSkeletalComponent;

class ISkeletalComponent abstract
{
public:
	virtual CSkeletalComponent* SkeletalComp() PURE;
};

END