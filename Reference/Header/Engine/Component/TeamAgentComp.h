#pragma once

#include "Component/GameObjectComp.h"


BEGIN(Engine)

/// <summary>
/// �Ҽ��� ������ ���� �ʿ��� ������Ʈ
/// �� ������Ʈ�� ������ �Ǹ� ��ȣ�ۿ�� �Ҽ����� ���ΰ��� ���迡 ���� ó���� �� �� �ִ�.
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