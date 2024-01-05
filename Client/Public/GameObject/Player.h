#pragma once

#include "GameObject/Character_Common.h"
#include "Client_Define.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)

class CTriBufferComp;
class CCommonModelComp;
class CColorShaderComp;
class CModelShaderComp;
class CNavigationComponent;

END



BEGIN(Client)

/// <summary>
/// �׽�Ʈ�� ������Ʈ
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

private:
	HRESULT	Initialize_Component();

public:		// �浹 �̺�Ʈ
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);


private:
	CCommonModelComp* m_pModelComp = { nullptr };
	CNavigationComponent* m_pNaviComp = { nullptr };

private:
	void	Move_Update(const _float& fTimeDelta);

private:
	_bool		m_bIsOnGround = true;
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

public:
	enum class EState_Act { Idle, Run, Walk, Ready_Jump, Jump_Up, Jump_Down, Landing, Buster };

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

private:
	void ShootBuster();

};

END