#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"
#include "GameObject/GameObjectFactory.h"

BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 잠자리형 리버봇
/// </summary>
class CReaverBot_Balfura : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CReaverBot_Balfura)
public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Balfura);

protected:
	explicit CReaverBot_Balfura();
	explicit CReaverBot_Balfura(const CReaverBot_Balfura& rhs);
	virtual ~CReaverBot_Balfura() = default;

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
	virtual void BeginPlay() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

public:
	static CReaverBot_Balfura* Create();
	static CReaverBot_Balfura* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;



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
	void Dead_Effect();

private:
	FGauge m_fDeadTime = FGauge(2.f);
	FGauge m_fDeadEffect = FGauge(0.1f);

	FGauge m_fHitTime = FGauge(0.1f);
	_float m_fHitStrength = { 0.3f };

private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, TurnRight, TurnLeft, LookTarget, LookHorizon, SlowMove, FastMove, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;
	_bool					m_bCanControl = { true };

public:
	enum class EState_Act { Idle, Dead };

	void Register_State();
	void Update_Move(const _float& fTimeDelta);

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	_float m_fSpeed = 5.f;
	_float m_fTurnSpeed = 5.f;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Dead(const _float& fTimeDelta);


public:
	enum class EState_AI { Idle, Look, SlowChase, Charge, Prowl };

private:		// 약식 상태머신
	using SState_AI = STATE_SET<EState_AI, void(ThisClass*, const _float&)>;
	SState_AI		m_State_AI;

	FGauge			m_fIdleTime = FGauge(3.f);
	FGauge			m_fTurnTime = FGauge(1.f);
	_bool			m_fTurnLeft = { true };

private:
	void AIState_Idle(const _float& fTimeDelta);
	void AIState_Look(const _float& fTimeDelta);
	void AIState_SlowChase(const _float& fTimeDelta);
	void AIState_Charge(const _float& fTimeDelta);
	void AIState_Prowl(const _float& fTimeDelta);

private:
	CCharacter_Common* Find_Target(_float fSearchDistance);

private:
	CCharacter_Common* m_pTarget = { nullptr };

};

template <>
struct TObjectExtTrait<CReaverBot_Balfura::g_ClassID>
{
	using Class = CReaverBot_Balfura;
};

END