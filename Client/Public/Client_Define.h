#pragma once

#include "System/GameInstance.h"

namespace Client 
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };
	const float g_fGravity = 9.8f * 1.30f;

	// 충돌 레이어, 마스크 전용, 이 설정으로 충돌쌍이 결정됨.
	enum ECOLLISION_LAYER
	{
		COLLAYER_CHARACTER = EBIT_FLAG32_0,
		COLLAYER_ATTACKER = EBIT_FLAG32_1,
		COLLAYER_OBJECT = EBIT_FLAG32_2,
		COLLAYER_ITEM = EBIT_FLAG32_3,
		COLLAYER_WALL = EBIT_FLAG32_4,
		COLLAYER_FLOOR = EBIT_FLAG32_5,
	};

	// 팀 설정 전용, 충돌 처리 후 충돌체에 대한 관계에 기반을 두고 설정하기 위함.
	enum ETEAM
	{
		ETEAM_PLAYER = EBIT_FLAG32_0,
		ETEAM_ENEMY = EBIT_FLAG32_1,
		ETEAM_NPC = EBIT_FLAG32_2,
		ETEAM_ATTACKABLE_NPC = EBIT_FLAG32_3,
	};
}
using namespace Client;

