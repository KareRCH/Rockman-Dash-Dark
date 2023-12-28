#pragma once

namespace Client 
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };
	const float g_fGravity = 9.8f * 1.30f;
}
using namespace Client;

#include "System/GameInstance.h"