#pragma once

#include "Client_Define.h"
#include "Character_Common.h"
#include "GameObject/GameObjectFactory.h"


BEGIN(Engine)

class CCommonModelComp;
class CCloudStationComp;
END


BEGIN(Client)

/// <summary>
/// 보스형 리버봇
/// </summary>
class CReaverBot_HanmuruDoll : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CReaverBot_HanmuruDoll)
public:
	static const _uint g_ClassID = ECast(EObjectIDExt::HanmuruDoll);

protected:
	explicit CReaverBot_HanmuruDoll();
	explicit CReaverBot_HanmuruDoll(const CReaverBot_HanmuruDoll& rhs);
	virtual ~CReaverBot_HanmuruDoll() = default;

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
	virtual void OnCreated() override;
	virtual void BeginPlay() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

public:
	static CReaverBot_HanmuruDoll* Create();
	static CReaverBot_HanmuruDoll* Create(FSerialData& InputData);
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
	CCloudStationComp* m_pCloudStationComp = { nullptr };

private:
	class CCloudStation_Boss* m_pBossCloud = { nullptr };

private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

private:
	FGauge m_fDeadTime = FGauge(5.f);
	FGauge m_fDeadEffect = FGauge(0.1f);
	FGauge m_fHitTime = FGauge(0.1f);
	_float m_fHitStrength = { 0.3f };

private:
	void Move_Update(const _float& fTimeDelta);
	void Update_CloudStation();
	void DeadEffect();

private:
	_float m_fMoveSpeed = { 5.f };

public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, TurnRight, TurnLeft, LookTarget, Smash, WalkAndSmash, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;
	_bool					m_bCanControl = { true };

public:
	enum class EState_Act { Idle, Walk, Ready_Smash, Smash, WalkAndSmash, Damaged, Dead, Stopped };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	FGauge m_fWalkAndSmash = FGauge(0.3f);

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Walk(const _float& fTimeDelta);
	void ActState_Ready_Smash(const _float& fTimeDelta);
	void ActState_Smash(const _float& fTimeDelta);
	void ActState_WalkAndSmash(const _float& fTimeDelta);
	void ActState_Damaged(const _float& fTimeDelta);
	void ActState_Dead(const _float& fTimeDelta);
	void ActState_Stopped(const _float& fTimeDelta);

public:
	enum class EState_AI { Idle, Chase, Smash, WalkAndSmash, WalkAndSmash_LookAt, OverHit, Dead, Escape };

private:		// 약식 상태머신
	using SState_AI = STATE_SET<EState_AI, void(ThisClass*, const _float&)>;
	SState_AI		m_State_AI;

	FGauge			m_fIdleTime = FGauge(3.f);
	FGauge			m_fTurnTime = FGauge(1.f);
	_bool			m_fTurnLeft = { true };
	_uint			m_fPhase	= { 0 };

private:
	void AIState_Idle(const _float& fTimeDelta);
	void AIState_Chase(const _float& fTimeDelta);
	void AIState_Smash(const _float& fTimeDelta);
	void AIState_WalkAndSmash(const _float& fTimeDelta);
	void AIState_WalkAndSmash_LookAt(const _float& fTimeDelta);
	void AIState_OverHit(const _float& fTimeDelta);
	void AIState_Dead(const _float& fTimeDelta);

private:
	CCharacter_Common* Find_Target();

private:
	CGameObject* m_pTarget = { nullptr };

private:
	void Create_DamageCollision();
	void Create_DamageCollisionBig();
};

template <>
struct TObjectExtTrait<CReaverBot_HanmuruDoll::g_ClassID>
{
	using Class = CReaverBot_HanmuruDoll;
};

END