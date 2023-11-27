#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

/// <summary>
/// Transform������Ʈ�� �����ϴ� ��ü�� �������ϴ� �������̽�
/// �ش� �Լ����� �����Ͽ� ���� �����־�� �Ѵ�.
/// </summary>
class ITransform abstract
{
public:
	inline virtual class CTransformComponent& Transform() PURE;
	inline virtual void Release_Transform() PURE;
};

END