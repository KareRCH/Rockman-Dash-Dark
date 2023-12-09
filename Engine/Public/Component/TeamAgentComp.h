#pragma once

#include "Component/GameObjectComp.h"

#include "Component/Interface/ITeamAgentComp.h"

BEGIN(Engine)

/// <summary>
/// �Ҽ��� ������ ���� �ʿ��� ������Ʈ
/// �� ������Ʈ�� ������ �Ǹ� ��ȣ�ۿ�� �Ҽ����� ���ΰ��� ���迡 ���� ó���� �� �� �ִ�.
/// Team ����� �̿��ϱ� ���ؼ��� ������ �� ���� ó�� ��ü�� �ʿ��ϴ�.
/// ������Ʈ�� ���� ID�� �������� ��ȣ���� ���� ������ ������.
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
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static CTeamAgentComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();

private:
	_uint	m_iTeamID;		// �� ID
	_uint	m_iPrivID;		// ���� ID, �� ������Ʈ ��
};

END