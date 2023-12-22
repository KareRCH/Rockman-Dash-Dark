#pragma once

#include "Engine_Macro.h"
#include "Engine_Typedef.h"

BEGIN(Engine)

enum class ECameraIndex : _uint
{
	One, Two, Three, Four, Size
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EViewportIndex : _uint
{
	One, Two, Three, Four, Five, Six, Seven, Eight, Size
};

// ����Ÿ�ٿ� ����Ʈ ����ü
enum class EViewportRT : _uint
{
	Normal, Depth, Height, Size
};

enum class ERenderGroup : _uint
{
	Priority,		// ���� ���� ���� ���� ������ �Ǵ� �׷�
	Alpha,			// ���� �׷�
	NonAlpha,		// ���� �ʿ���� ���۵� �׷�
	UI,				// UI �׷�
	PostProcess,	// ��ó�� �׷�
	Size
};

END