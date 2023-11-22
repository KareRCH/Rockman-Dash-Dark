#pragma once

#include "Component/GameObjectComp.h"


BEGIN(Engine)

/// <summary>
/// 소속을 가지기 위해 필요한 컴포넌트
/// 이 컴포넌트는 프리미티브로 분류됩니다.
/// </summary>
class ENGINE_DLL CTeamAgentComp final : public CGameObjectComp
{

};

END