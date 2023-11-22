#pragma once

#include "Component/GameObjectComp.h"


BEGIN(Engine)

/// <summary>
/// 소속을 가지기 위해 필요한 컴포넌트
/// 이 컴포넌트를 가지게 되면 상호작용시 소속팀과 서로간의 관계에 의한 처리를 할 수 있다.
/// </summary>
class ENGINE_DLL CTeamAgentComp final : public CGameObjectComp
{
	DERIVED_CLASS(CInternalComponent, CTeamAgentComp)
protected:
	explicit CTeamAgentComp() = default;
	explicit CTeamAgentComp(const CTeamAgentComp& rhs);
	virtual ~CTeamAgentComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual void	Render();

public:
	static CTeamAgentComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();
};

END