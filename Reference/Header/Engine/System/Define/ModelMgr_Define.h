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

/// <summary>
/// �Ƹ��߾�� ���� �����ϱ� ���� ������� ����ü
/// </summary>
enum class EModelNodeType : _uint
{
	Armature,
	Bone,
	Size
};

/// <summary>
/// ���۰� ���� �����ϱ� ���� ���� ����ü.
/// ���̶�� End�� �����Ǿ� �׷��� ǥ�ø� �� �� �� ǥ�ø� ���ְ� �ȴ�.
/// </summary>
enum class EModelBoneType : _uint
{
	Null,
	Base,
	End
};

END