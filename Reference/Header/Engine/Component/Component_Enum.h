#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

enum class ECOMP_UPDATE_TYPE : _uint
{
	MANUAL,
	SEMI_AUTO,
};
using ECOMP_UPDATE_T = ECOMP_UPDATE_TYPE;

END