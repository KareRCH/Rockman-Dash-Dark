#pragma once

#include "Engine_Macro.h"
#include "Engine_Typedef.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint
{
	System,			// �ý��ۿ��� ���������� ���̴� ���� ��ϵǴ� �ε��� (�ε�)
	Permanent,		// ĳ���Ϳ� �ý��� ������ �ƴϳ�, ���������� ���Ǵ�(�÷��̾�) ���� �����Ͱ� ��ϵǴ� �ε���
	Level,			// �������� �Ҵ� ���� �Ǵ� ���� ��ϵǴ� �ε���, �� ���������� ���̴� ���� ���
	Size
};

END