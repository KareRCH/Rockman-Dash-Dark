#pragma once

#include "Client_Define.h"
#include "GameObject/Character.h"
#include "Component/TeamAgentComp.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

/// <summary>
/// 3D 월드를 돌아다니는 일반적인 캐릭터 클래스
/// </summary>
class CCharacter_Common abstract : public CCharacter
{
	DERIVED_CLASS(CCharacter, CCharacter_Common)

protected:
	explicit CCharacter_Common();
	explicit CCharacter_Common(const CCharacter_Common& rhs);
	virtual ~CCharacter_Common() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual CGameObject*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void			Free() override;

private:
	HRESULT			Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);


#pragma region 팀 에이전트
public:
	CTeamAgentComp& TeamAgentComp() const { return *m_pTeamAgentComp; }

private:
	CTeamAgentComp* m_pTeamAgentComp = { nullptr };
#pragma endregion


protected:
	FGauge	m_fHP = FGauge(100.f);
	//FGauge

};

END