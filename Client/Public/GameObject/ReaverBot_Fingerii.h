#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"

#include "Utility/LogicDeviceBasic.h"
#include "GameObject/GameObjectFactory.h"


BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 가장 기본이 되는 몬스터 리버봇, 호로꼬
/// </summary>
class CReaverBot_Fingerii : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CReaverBot_Fingerii)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Fingerii);

protected:
	explicit CReaverBot_Fingerii();
	explicit CReaverBot_Fingerii(const CReaverBot_Fingerii& rhs);
	virtual ~CReaverBot_Fingerii() = default;

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
	static CReaverBot_Fingerii* Create();
	static CReaverBot_Fingerii* Create(FSerialData& InputData);
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
	_float m_fMoveSpeed = { 3.f };


public:
	void Dead_Effect();


public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, TurnRight, TurnLeft, LookTarget, Barrier, Concentration, Size };
	
private:
	ACTION_SET<EActionKey>	m_ActionKey;
	_bool					m_bCanControl = { true };

public:
	enum class EState_Act { Idle, Walk, Barrier, Concentration, Dead };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	FGauge m_fDeadTime = FGauge(2.f);
	FGauge m_fDeadEffect = FGauge(0.1f);
	FGauge m_fEnergyBallAttack = FGauge(5.f);

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Walk(const _float& fTimeDelta);
	void ActState_Barrier(const _float& fTimeDelta);
	void ActState_Concentration(const _float& fTimeDelta);
	void ActState_Dead(const _float& fTimeDelta);

private:
	void Create_EnergyBall();

public:
	enum class EState_AI { Idle, Chase, Barrier, EnergyBall, Prowl, Dead, Escape };

private:		// 약식 상태머신
	using SState_AI = STATE_SET<EState_AI, void(ThisClass*, const _float&)>;
	SState_AI		m_State_AI;

	FGauge			m_fIdleTime = FGauge(3.f);
	FGauge			m_fTurnTime = FGauge(1.f);
	_bool			m_fTurnLeft = { true };

private:
	void AIState_Idle(const _float& fTimeDelta);
	void AIState_Chase(const _float& fTimeDelta);
	void AIState_Barrier(const _float& fTimeDelta);
	void AIState_EnergyBall(const _float& fTimeDelta);
	void AIState_Prowl(const _float& fTimeDelta);
	void AIState_Dead(const _float& fTimeDelta);
	void AIState_Escape(const _float& fTimeDelta);


private:
	CCharacter_Common* Find_Target();

private:
	CGameObject*	m_pTarget = { nullptr };
	
};

template <>
struct TObjectExtTrait<CReaverBot_Fingerii::g_ClassID>
{
	using Class = CReaverBot_Fingerii;
};

END