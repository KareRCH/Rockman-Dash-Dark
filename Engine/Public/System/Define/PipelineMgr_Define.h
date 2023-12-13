#pragma once

#include "Engine_Macro.h"
#include "Engine_Typedef.h"

BEGIN(Engine)

enum class ECamType : _uint
{
	Ortho, Persp, Size
};

enum class ECamMatrix : _uint
{
	View, Proj, Size
};

enum class ECamNum : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

enum class EViewportNum : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

END