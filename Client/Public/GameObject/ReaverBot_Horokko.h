#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"

#include "Utility/LogicDeviceBasic.h"
#include "GameObject/GameObjectFactory.h"
#include "Utility/ClassID.h"


BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 가장 기본이 되는 몬스터 리버봇, 호로꼬
/// </summary>
class CReaverBot_Horokko : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CReaverBot_Horokko)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Horokko);

protected:
	explicit CReaverBot_Horokko();
	explicit CReaverBot_Horokko(const CReaverBot_Horokko& rhs);
	virtual ~CReaverBot_Horokko() = default;

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
	static CReaverBot_Horokko* Create();
	static CReaverBot_Horokko* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	HRESULT	Initialize_Component(FSerialData& InputData);


public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	void Input_ActionKey();
	void Move_Update(const _float& fTimeDelta);

private:
	_float m_fMoveSpeed = { 5.f };


public:
	void Dead_Effect();


public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, TurnRight, TurnLeft, LookTarget, Charge, ReadyBomb, ShootBomb, Size };
	
private:
	ACTION_SET<EActionKey>	m_ActionKey;
	_bool					m_bCanControl = { true };

public:
	enum class EState_Act { Idle, Run, Ready_Charge, Charge_Attack, Ready_Shooting, Shooting, End_Shooting, Dead };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	FGauge m_fDeadTime = FGauge(2.f);
	FGauge m_fDeadEffect = FGauge(0.1f);

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Run(const _float& fTimeDelta);
	void ActState_Ready_Charge(const _float& fTimeDelta);
	void ActState_Charge_Attack(const _float& fTimeDelta);
	void ActState_Ready_Shooting(const _float& fTimeDelta);
	void ActState_Shooting(const _float& fTimeDelta);
	void ActState_End_Shooting(const _float& fTimeDelta);
	void ActState_Dead(const _float& fTimeDelta);


public:
	enum class EState_AI { Idle, Chase, Charge, Charge_Attack, Prowl, Dead, Escape };

private:		// 약식 상태머신
	using SState_AI = STATE_SET<EState_AI, void(ThisClass*, const _float&)>;
	SState_AI		m_State_AI;

	FGauge			m_fIdleTime = FGauge(3.f);
	FGauge			m_fTurnTime = FGauge(1.f);
	_bool			m_fTurnLeft = { true };

private:
	void AIState_Idle(const _float& fTimeDelta);
	void AIState_Chase(const _float& fTimeDelta);
	void AIState_Charge(const _float& fTimeDelta);
	void AIState_Charge_Attack(const _float& fTimeDelta);
	void AIState_Prowl(const _float& fTimeDelta);
	void AIState_Dead(const _float& fTimeDelta);
	void AIState_Escape(const _float& fTimeDelta);


private:
	CCharacter_Common* Find_Target();

private:
	CGameObject*	m_pTarget = { nullptr };
	
};

template <>
struct TObjectExtTrait<CReaverBot_Horokko::g_ClassID>
{
	using Class = CReaverBot_Horokko;
};

END