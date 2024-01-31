#pragma once

#include "GameObject/Character_Common.h"
#include "Client_Define.h"

#include "Utility/LogicDeviceBasic.h"
#include "Component/PivotComponent.h"

#include "GameObject/GameObjectFactory.h"

BEGIN(Engine)

class CCommonModelComp;
class CNavigationComponent;
class CCloudStationComp;

END



BEGIN(Client)

class CCloudStation_Player;

static const _tchar* g_PrototypePath = TEXT("Resource/Prototypes/");

/// <summary>
/// 테스트용 오브젝트
/// </summary>
class CPlayer final : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CPlayer)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Player);

protected:
	explicit CPlayer();
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

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
	static CPlayer* Create();
	static CPlayer* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	// 시리얼 데이터로 초기화시 사용
	HRESULT	Initialize_Component(FSerialData& InputData);

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

public:
	void Update_ToCloudStation();

private:
	CCommonModelComp* m_pModelComp = { nullptr };
	CNavigationComponent* m_pNaviComp = { nullptr };
	CPivotComponent* m_pCameraPivotComp = { nullptr };
	CCloudStationComp* m_pCloudStationComp = { nullptr };

private:
	CCloudStation_Player* m_pPlayerCloud = { nullptr };

private:
	void	Register_State();
	void	Move_Update(const _float& fTimeDelta);
	void	Look_Update(const _float& fTimeDelta);
	void	Input_Move(const _float& fTimeDelta);
	void	Input_Weapon(const _float& fTimeDelta);

private:
	_bool		m_bIsMoving = false;

	
	_float3		m_vLookDirection = { 0.f, 0.f, 1.f};		// 캐릭터가 바라보는 방향
	_float3		m_vLookDirection_Blend = { 0.f, 0.f, 1.f};	// 캐릭터가 바라보는 방향 블렌딩용
	_float		m_fDirectionAngle = 0.f;				// 캐릭터가 바라보는 방향각도

	FGauge		m_fFootSound = FGauge(0.26f);
	FGauge		m_fDamageKnockback = FGauge(0.5f);		// 넉백 시간
	FGauge		m_fStrongKnockback = FGauge(1.f);		// 강하게 넉백

	FGauge		m_fKnockDownTime = FGauge(2.f);			// 넉다운 시간
	FGauge		m_fKnockDownValue = FGauge(3.f);		// 넉다운 되기 위한 수치
	FDelay		m_fKnockDownDelay = FDelay(0.7f);		// 넉다운 딜레이

	_bool		m_bInvisible = { false };				// 무적
	FDelay		m_fInvisibleTime = FDelay(5.f, true);	// 무적시간
	FGauge		m_fGauge = FGauge(1.f);					// 범용 타이머

	enum MOVE_DIR { MOVE_LEFT, MOVE_RIGHT, MOVE_FORWARD, MOVE_BACK };
	MOVE_DIR	m_ePrevMoveDir = { MOVE_FORWARD };
	MOVE_DIR	m_eMoveDir = { MOVE_FORWARD };
	_bool		m_bIsCanMove = { true };

public:
	enum class EActionKey : _uint { MoveForward, MoveBackward, MoveRight, MoveLeft, Jump, Lockon,
		MoveSlow, MoveFast, JumpLow, LowFrict,
		Buster, ChargeBuster, 
		Laser, Homing, Blade, BusterCannon, Drill, HyperShell, Machinegun, Shield, SpreadBuster, 
		Grab, Throw, Squat, 
		Interaction,
		Size 
	};

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Idle, Run, Walk, Ready_Jump, Jump_Up, Jump_Down, Landing, Buster, 
		DamagedLight, DamagedHeavy, KnockDown, StandUp, ReadyLaser, ShootingLaser, EndLaser,
		Homing, SpreadBuster, ChargeShot,
		ReadyBusterCannon, ShootBusterCannon, EndBusterCannon,
		ReadyHyperShell, ShootHyperShell, EndHyperShell,
		ReadyMachinegun, ShootingMachinegun, EndMachinegun,
		BladeAttack1, BladeAttack2, BladeEnd,
		Grab, Grabbing, Throw, Squat,
		ItemGetting, ItemGet,
	};

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

	void ActState_Homing(const _float& fTimeDelta);

	void ActState_SpreadBuster(const _float& fTimeDelta);
	void ActState_ChargeShot(const _float& fTimeDelta);

	void ActState_ReadyBusterCannon(const _float& fTimeDelta);
	void ActState_ShootBusterCannon(const _float& fTimeDelta);
	void ActState_EndBusterCannon(const _float& fTimeDelta);

	void ActState_ReadyHyperShell(const _float& fTimeDelta);
	void ActState_ShootHyperShell(const _float& fTimeDelta);
	void ActState_EndHyperShell(const _float& fTimeDelta);

	void ActState_ReadyMachinegun(const _float& fTimeDelta);
	void ActState_ShootingMachinegun(const _float& fTimeDelta);
	void ActState_EndMachinegun(const _float& fTimeDelta);

	void ActState_BladeAttack1(const _float& fTimeDelta);
	void ActState_BladeAttack2(const _float& fTimeDelta);
	void ActState_BladeEnd(const _float& fTimeDelta);

	void ActState_Grab(const _float& fTimeDelta);
	void ActState_Grabbing(const _float& fTimeDelta);
	void ActState_Throw(const _float& fTimeDelta);
	void ActState_Squat(const _float& fTimeDelta);
	void ActState_ItemGetting(const _float& fTimeDelta);
	void ActState_ItemGet(const _float& fTimeDelta);

private:
	void ShootBuster();
	void ShootMissile();
	void ShootSpreadBuster();
	void ShootLaser();
	void AttachLaser();
	void DeleteLaser();
	void ShootBusterCannon();
	void ShootHyperShell();
	void ShootMachinegun();
	void CreateBlade();
	void AttachBlade();
	void DeleteBlade();
	void GrabingUnit();
	void ThrowUnit();
	void Gotcha(_bool bIsGetItem, EItemObtain eItem);
	void EndOpenChest();
	void Lockon_Active(const _float& fTimeDelta);
	void Lockon_Target();
	void Lockon_Untarget();

public:
	void Set_Target(class CCharacter_Common* pTarget) { m_pLockon_Target = pTarget; Safe_AddRef(m_pLockon_Target); }

private:
	CCharacter_Common* Find_Target(_float fRange);

private:
	class CCharacter_Common* m_pLockon_Target = { nullptr };
	class CUI_Lockon* m_pLockon_UI = { nullptr };

public:
	void ChangeMainWeapon(EMainWeapon eWeapon);
	void ChangeSubWeapon(ESubWeapon eWeapon);

private:
	EMainWeapon			m_eMainWeapon = { EMainWeapon::None };

private:
	ESubWeapon			m_eSubWeapon = { ESubWeapon::ThrowArm };

private:
	class CWeapon_LaserEmission*		m_pLaserEmission = { nullptr };
	class CWeapon_Blade*				m_pBlade = { nullptr };
	CCharacter_Common*					m_pGrabUnit = { nullptr };

};

template <>
struct TObjectExtTrait<CPlayer::g_ClassID>
{
	using Class = CPlayer;
};

END