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

enum class EGObjectState : _uint
{
	Dead = EBIT_FLAG32_0,
	Pause = EBIT_FLAG32_1,
	Render = EBIT_FLAG32_2,				// ������ ����
	RenderZBuffer = EBIT_FLAG32_3,		// ZBuffer�� �������ϴ� �ɼ�
	RenderDeferred = EBIT_FLAG32_4,		// Defferred�� �������ϴ� �ɼ� (������), ���� ������ �ߵǾ��־�� �۵�
	RenderPriority = EBIT_FLAG32_5,		// ZBuffer�� �ʿ���� ���� ���� ������ �Ǵ� ��ī�� �ڽ���
	RenderPostProcess = EBIT_FLAG32_6	// ZBuffer�� �ʿ���� ���� ���߿� ������ �Ǵ� ����Ʈ ���μ�����
};

END