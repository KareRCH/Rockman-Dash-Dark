#pragma once

#include "Client_Define.h"
#include "GameObject/Character.h"
#include "Component/TeamAgentComp.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

/// <summary>
/// 3D ���带 ���ƴٴϴ� �Ϲ����� ĳ���� Ŭ����
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
	virtual void BeginPlay() override;

public:
	virtual CGameObject*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void			Free() override;

private:
	HRESULT			Initialize_Component();

public:		// �浹 �̺�Ʈ
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);


#pragma region �� ������Ʈ
public:
	CTeamAgentComp& TeamAgentComp() const { return *m_pTeamAgentComp; }

private:
	CTeamAgentComp* m_pTeamAgentComp = { nullptr };
#pragma endregion

private:
	_bool		m_bIsPossessed = { false };			// ���� �Ǿ� ������ ������
	_bool		m_bIsLookOnGround = { true };		// ī�޶� ��ü�� Right ���� ȸ���� ����

public:
	_bool	Get_IsCanGrab() const { return m_bIsCanGrab; }
	void	Set_Grabbed(_bool bIsGrabbed) { m_bIsGrabbed = bIsGrabbed; }
	void	Set_Throwing(_bool bIsThrowing) { m_bIsThrowing = bIsThrowing; }
	void	Set_Velocity(_float3 vVelocity) { m_vVelocity = vVelocity; }
	_bool	Get_IsCharacterDead() const { return m_bIsDead; }

protected:
	_bool		m_bIsDead = { false };					// object�� �״� �Ͱ� ������ ��� ó���� ����
	_bool		m_bIsOnGround = { true };
	_bool		m_bIsCanGrab = { false };
	_bool		m_bIsGrabbed = { false };
	_bool		m_bIsThrowing = { false };

	_float3		m_vMaxMoveSpeed = {};					// �ִ� ������ �ӵ�
	_float3		m_vMoveSpeed = {};						// ������ �ӵ�
	_float3		m_vVelocity = {};						// �ӵ�
	_float3		m_vAcceleration = {};					// ���ӵ�

public:
	void Damage_HP(_float fDamage) { m_fHP.fCur -= fDamage; }

protected:
	FGauge	m_fHP = FGauge(100.f, true);
	//FGauge

};

END