#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

/// <summary>
/// Transform컴포넌트를 포함하는 객체가 가져야하는 인터페이스
/// 해당 함수들을 구현하여 연결 시켜주어야 한다.
/// </summary>
class ITransform abstract
{
public:
	inline virtual class CTransformComponent& Transform() PURE;
	inline virtual void Release_Transform() PURE;
};

END