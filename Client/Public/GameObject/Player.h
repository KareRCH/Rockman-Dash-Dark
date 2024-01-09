#pragma once

#include "GameObject/Character_Common.h"
#include "Client_Define.h"

#include "Utility/LogicDeviceBasic.h"
#include "Component/PivotComponent.h"

BEGIN(Engine)

class CTriBufferComp;
class CCommonModelComp;
class CColorShaderComp;
class CModelShaderComp;
class CNavigationComponent;

END



BEGIN(Client)

/// <summary>
/// 테스트용 오브젝트
/// </summary>
class CPlayer final : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CPlayer)

protected:
	explicit CPlayer();
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CPlayer* Create();
	static CPlayer* Create(const _float3 vPos);
	virtual CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);


private:
	CCommonModelComp* m_pModelComp = { nullptr };
	CNavigationComponent* m_pNaviComp = { nullptr };
	CPivotComponent* m_pCameraPivotComp = { nullptr };

private:
	void	Register_State();
	void	Move_Update(const _float& fTimeDelta);
	void	Look_Update(const _float& fTimeDelta);

private:
	_bool		m_bIsOnGround = true;
	_bool		m_bIsMoving = false;

	_float3		m_vMaxMoveSpeed = {};					// 최대 움직임 속도
	_float3		m_vMoveSpeed = {};						// 움직임 속도
	_float3		m_vVelocity = {};						// 속도
	_float3		m_vAcceleration = {};					// 가속도
	_float3		m_vLookDirection = { 0.f, 0.f, 1.f};		// 캐릭터가 바라보는 방향
	_float3		m_vLookDirection_Blend = { 0.f, 0.f, 1.f};	// 캐릭터가 바라보는 방향 블렌딩용
	_float		m_fDirectionAngle = 0.f;				// 캐릭터가 바라보는 방향각도

	FGauge		m_fFootSound = FGauge(0.25f);
	FGauge		m_fDamageKnockback = FGauge(0.5f);		// 넉백 시간
	FGauge		m_fStrongKnockback = FGauge(1.f);		// 강하게 넉백

	FGauge		m_fKnockDownTime = FGauge(2.f);			// 넉다운 시간
	FGauge		m_fKnockDownValue = FGauge(3.f);		// 넉다운 되기 위한 수치
	FDelay		m_fKnockDownDelay = FDelay(0.7f);		// 넉다운 딜레이

	_bool		m_bInvisible = { false };				// 무적
	FDelay		m_fInvisibleTime = FDelay(5.f, true);	// 무적시간

	enum MOVE_DIR { MOVE_LEFT, MOVE_RIGHT, MOVE_FORWARD, MOVE_BACK };
	MOVE_DIR	m_ePrevMoveDir = { MOVE_FORWARD };
	MOVE_DIR	m_eMoveDir = { MOVE_FORWARD };

public:
	enum class EState_Act { Idle, Run, Walk, Ready_Jump, Jump_Up, Jump_Down, Landing, Buster, 
		DamagedLight, DamagedHeavy, KnockDown, StandUp, ReadyLaser, ShootingLaser, EndLaser };

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Run(const _float& fTimeDelta);
	void ActState_Walk(const _float& fTimeDelta);
	void ActState_Ready_Jump(const _float& fTimeDelta);
	void ActState_Jump_Up(const _float& fTimeDelta);
	void ActState_Jump_Down(const _float& fTimeDelta);
	void ActState_Landing(const _float& fTimeDelta);
	void ActState_Buster(const _float& fTimeDelta);
	void ActState_DamagedLight(const _float& fTimeDelta);
	void ActState_DamagedHeavy(const _float& fTimeDelta);
	void ActState_KnockDown(const _float& fTimeDelta);
	void ActState_StandUp(const _float& fTimeDelta);
	void ActState_ReadyLaser(const _float& fTimeDelta);
	void ActState_ShootingLaser(const _float& fTimeDelta);
	void ActState_EndLaser(const _float& fTimeDelta);

private:
	void ShootBuster();

};

END