#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

enum class EModelGroupIndex : _uint
{
	System,			// �ý��ۿ��� ���������� ���̴� ���� ��ϵǴ� �ε��� (�ε�)
	Permanent,		// ĳ���Ϳ� �ý��� ������ �ƴϳ�, ���������� ���Ǵ�(�÷��̾�) ���� �����Ͱ� ��ϵǴ� �ε���
	Level,			// �������� �Ҵ� ���� �Ǵ� ���� ��ϵǴ� �ε���, �� ���������� ���̴� ���� ���
	Size
};

END