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

enum class EGObjectState : _uint
{
	Dead = EBIT_FLAG32_0,
	Pause = EBIT_FLAG32_1,
	Render = EBIT_FLAG32_2,				// 렌더링 유무
	RenderZBuffer = EBIT_FLAG32_3,		// ZBuffer로 렌더링하는 옵션
	RenderDeferred = EBIT_FLAG32_4,		// Defferred로 렌더링하는 옵션 (불투명), 내부 세팅이 잘되어있어야 작동
	RenderPriority = EBIT_FLAG32_5,		// ZBuffer가 필요없고 가장 먼저 렌더링 되는 스카이 박스류
	RenderPostProcess = EBIT_FLAG32_6	// ZBuffer가 필요없고 가장 나중에 렌더링 되는 포스트 프로세스류
};

END