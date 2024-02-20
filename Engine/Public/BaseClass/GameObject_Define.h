#pragma once

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Enum.h"

BEGIN(Engine)

enum class EGObjTickPriority : _uint
{
	Tick,			// ������Ʈ �Ҷ� �켱��
	Render,			// ������ �Ҷ� �켱�� (ZBuffer �̻�� ��ü�� �����ϴµ� ���), ZBuffer ���� ��ü�� Z������ ���, �� ���� ������ Z������ �켱�� ����
	Size
};

enum class EGObjTag : _uint
{
	Common,		// �Ϲ� �з� �±�, �Ϲ� ���� ������ ���δ�.
	Level,		// ���� �з� �±�, ��� ������ �����ִ����� ���� ���δ�. �������� �ٲ� �� �ִ�.
	Layer,		// ���̾� �з� �±�, �����Ϳ��� ���δ�.
	Size
};

enum class EGObjectState : _uint
{
	Dead = EBIT_FLAG32_0,
	Pause = EBIT_FLAG32_1,				// ���� ����
	Priority_Tick = EBIT_FLAG32_2,		// �켱 ƽ
	Tick = EBIT_FLAG32_3,				// ƽ
	Late_Tick = EBIT_FLAG32_4,			// ����Ʈ ƽ
	Render = EBIT_FLAG32_5,				// ������ ����
	RenderZBuffer = EBIT_FLAG32_6,		// ZBuffer�� �������ϴ� �ɼ�
	Cull = EBIT_FLAG32_7,				// �ø� �ɼ�
	Drawing = EBIT_FLAG32_8,			// ���� �׷����� �ִ°�
	Shadow = EBIT_FLAG32_9,				// �׸��� �׸��� �ɼ�
	Tool = EBIT_FLAG32_10,				// �� �±�
	InstantDelete = EBIT_FLAG32_11,		// ��� ���� �±�, EndPlay �۵� ���ϵ�����.
	Tick_Pause = EBIT_FLAG32_12,		// ƽ�� ����
};

END