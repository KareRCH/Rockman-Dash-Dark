#pragma once

#include "Engine_Macro.h"

BEGIN(Engine)

class CTeamAgentComp;

class ITeamAgentComp abstract
{
public:
	virtual CTeamAgentComp* TeamAgentComp() = 0;
};

END