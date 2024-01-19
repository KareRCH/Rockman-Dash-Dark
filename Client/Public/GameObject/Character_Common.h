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

protected:
	_bool		m_bIsOnGround = { false };

public:
	void Damage_HP(_float fDamage) { m_fHP.fCur -= fDamage; }

protected:
	FGauge	m_fHP = FGauge(100.f, true);
	//FGauge

};

END