#pragma once

#include "Engine_Macro.h"
#include "Engine_Typedef.h"

BEGIN(Engine)

enum class ECameraIndex : _uint
{
	One, Two, Three, Four, Size
};

// 렌더타겟용 뷰포트 열거체
enum class EViewportIndex : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

// 렌더타겟용 뷰포트 열거체
enum class EViewportRT : _uint
{
	Normal, Depth, Height, Size
};

enum class ERenderGroup : _uint
{
	Priority,		// 버퍼 없이 가장 먼저 렌더링 되는 그룹
	Alpha,			// 알파 그룹
	NonAlpha,		// 알파 필요없는 디퍼드 그룹
	UI,				// UI 그룹
	PostProcess,	// 후처리 그룹
	Size
};

END