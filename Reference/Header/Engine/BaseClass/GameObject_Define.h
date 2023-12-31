#pragma once

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Enum.h"

BEGIN(Engine)

enum class EGObjTickPriority : _uint
{
	Tick,			// 업데이트 할때 우선도
	Render,			// 렌더링 할때 우선도 (ZBuffer 미사용 객체만 정렬하는데 사용), ZBuffer 쓰는 객체는 Z값으로 계산, 이 값이 같으면 Z값으로 우선도 결정
	Size
};

enum class EGObjTag : _uint
{
	Common,		// 일반 분류 태그, 일반 게임 로직에 쓰인다.
	Level,		// 레벨 분류 태그, 어느 레벨에 속해있는지에 대해 쓰인다. 동적으로 바뀔 수 있다.
	Layer,		// 레이어 분류 태그, 에디터에서 쓰인다.
	Size
};

enum class EGObjectState : _uint
{
	Dead = EBIT_FLAG32_0,
	Pause = EBIT_FLAG32_1,
	Priority_Tick = EBIT_FLAG32_2,		// 우선 틱
	Tick = EBIT_FLAG32_3,				// 틱
	Late_Tick = EBIT_FLAG32_4,			// 레이트 틱
	Render = EBIT_FLAG32_5,				// 렌더링 유무
	RenderZBuffer = EBIT_FLAG32_6,		// ZBuffer로 렌더링하는 옵션
};

END