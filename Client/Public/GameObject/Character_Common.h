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
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(FSerialData& InputData);
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

private:
	_bool		m_bIsPossessed = { false };			// 빙의 되어 조종이 가능함
	_bool		m_bIsLookOnGround = { true };		// 카메라가 물체의 Right 벡터 회전을 무시

protected:
	_bool		m_bIsOnGround = { false };

public:
	void Damage_HP(_float fDamage) { m_fHP.fCur -= fDamage; }

protected:
	FGauge	m_fHP = FGauge(100.f, true);
	//FGauge

};

END