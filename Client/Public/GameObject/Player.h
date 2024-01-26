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
/// �׽�Ʈ�� ������Ʈ
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
	// �ø��� �����ͷ� �ʱ�ȭ�� ���
	HRESULT	Initialize_Component(FSerialData& InputData);

public:		// �浹 �̺�Ʈ
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
	void	Input_Weapon(const _float& fTimeDelta);

private:
	_bool		m_bIsMoving = false;

	_float3		m_vMaxMoveSpeed = {};					// �ִ� ������ �ӵ�
	_float3		m_vMoveSpeed = {};						// ������ �ӵ�
	_float3		m_vVelocity = {};						// �ӵ�
	_float3		m_vAcceleration = {};					// ���ӵ�
	_float3		m_vLookDirection = { 0.f, 0.f, 1.f};		// ĳ���Ͱ� �ٶ󺸴� ����
	_float3		m_vLookDirection_Blend = { 0.f, 0.f, 1.f};	// ĳ���Ͱ� �ٶ󺸴� ���� ������
	_float		m_fDirectionAngle = 0.f;				// ĳ���Ͱ� �ٶ󺸴� ���Ⱒ��

	FGauge		m_fFootSound = FGauge(0.25f);
	FGauge		m_fDamageKnockback = FGauge(0.5f);		// �˹� �ð�
	FGauge		m_fStrongKnockback = FGauge(1.f);		// ���ϰ� �˹�

	FGauge		m_fKnockDownTime = FGauge(2.f);			// �˴ٿ� �ð�
	FGauge		m_fKnockDownValue = FGauge(3.f);		// �˴ٿ� �Ǳ� ���� ��ġ
	FDelay		m_fKnockDownDelay = FDelay(0.7f);		// �˴ٿ� ������

	_bool		m_bInvisible = { false };				// ����
	FDelay		m_fInvisibleTime = FDelay(5.f, true);	// �����ð�

	enum MOVE_DIR { MOVE_LEFT, MOVE_RIGHT, MOVE_FORWARD, MOVE_BACK };
	MOVE_DIR	m_ePrevMoveDir = { MOVE_FORWARD };
	MOVE_DIR	m_eMoveDir = { MOVE_FORWARD };

public:
	enum class EActionKey : _uint { Buster, ChargeBuster, Throw, 
		Laser, Homing, Blade, BusterCannon, Drill, HyperShell, Machinegun, Shield, SpreadBuster, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Idle, Run, Walk, Ready_Jump, Jump_Up, Jump_Down, Landing, Buster, 
		DamagedLight, DamagedHeavy, KnockDown, StandUp, ReadyLaser, ShootingLaser, EndLaser,
		Homing, SpreadBuster, ChargeShot };

private:		// ��� ���¸ӽ�
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

private:
	void ShootBuster();
	void ShootMissile();
	void ShootSpreadBuster();
	void Lockon_Active(const _float& fTimeDelta);
	void Lockon_Target();
	void Lockon_Untarget();

	

public:
	void Set_Target(class CCharacter_Common* pTarget) { m_pLockon_Target = pTarget; Safe_AddRef(m_pLockon_Target); }

private:
	CCharacter_Common* Find_Target();

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

};

template <>
struct TObjectExtTrait<CPlayer::g_ClassID>
{
	using Class = CPlayer;
};

END