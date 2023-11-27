#pragma once

#include "Engine_Typedef.h"
#include "Engine_Macro.h"

BEGIN(Engine)

enum class ECompTickType : _uint
{
	Priority,
	Tick,
	Late,
	Render,
	Size
};

enum class ECompTickAuto : _uint
{
	SemiAuto,
	Manual,
};

END