#pragma once

namespace Tool {}
using namespace Tool;

#ifndef TOOL
#define TOOL
#endif

#include "System/GameInstance.h"


const unsigned int G_iViewerViewportIndex = 0;


struct SHADER_MACRO_TOOL
{
	static constexpr D3D_SHADER_MACRO Desc[] = {
		{ "TOOL", "0" },
		{ nullptr, nullptr }
	};
};
