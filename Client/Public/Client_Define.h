#pragma once

#include "System/GameInstance.h"

namespace Client 
{
	enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };
	const float g_fGravity = 9.8f * 1.30f;

	// �浹 ���̾�, ����ũ ����, �� �������� �浹���� ������.
	enum ECOLLISION_LAYER
	{
		COLLAYER_CHARACTER = EBIT_FLAG32_0,
		COLLAYER_ATTACKER = EBIT_FLAG32_1,
		COLLAYER_OBJECT = EBIT_FLAG32_2,
		COLLAYER_ITEM = EBIT_FLAG32_3,
		COLLAYER_WALL = EBIT_FLAG32_4,
		COLLAYER_FLOOR = EBIT_FLAG32_5,
	};

	// �� ���� ����, �浹 ó�� �� �浹ü�� ���� ���迡 ����� �ΰ� �����ϱ� ����.
	enum ETEAM
	{
		ETEAM_PLAYER = EBIT_FLAG32_0,
		ETEAM_ENEMY = EBIT_FLAG32_1,
		ETEAM_NPC = EBIT_FLAG32_2,
		ETEAM_ATTACKABLE_NPC = EBIT_FLAG32_3,
	};
}
using namespace Client;

