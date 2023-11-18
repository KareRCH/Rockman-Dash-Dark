#pragma once

#include "Engine_Macro.h"
#include "Engine_Typedef.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint
{
	System,			// 시스템에서 전역적으로 쓰이는 모델이 등록되는 인덱스 (로딩)
	Permanent,		// 캐릭터와 시스템 전용은 아니나, 영구적으로 사용되는(플레이어) 모델의 데이터가 등록되는 인덱스
	Level,			// 동적으로 할당 해제 되는 모델이 등록되는 인덱스, 그 레벨에서만 쓰이는 모델의 경우
	Size
};

END