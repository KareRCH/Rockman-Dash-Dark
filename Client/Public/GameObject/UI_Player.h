#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

class CUI_Player : public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_Player)

protected:
	explicit CUI_Player();
	explicit CUI_Player(const CUI_Player& rhs);
	virtual ~CUI_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CUI_Player* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();


private:
	FGauge			m_fHP;						// HP
	FGauge			m_fHP_Delayed;				// HP ������ ��
	FDelay			m_fHP_ReduceTime;			// HP ��� �����ð�

	FGauge			m_fSubWeapon_Main;			// ���� ��ź��
	FGauge			m_fSubWeapon_Usage;			// ���� ��� ������ ��ź
	FGauge			m_fSubWeapon_Usage_Delayed;	// ���� ��� ���� ��ź ������ ó��

	_bool			m_bIsEnemyDetected;			// �þ� �ۿ��� ���� ���� �ȿ� ���� �� Ȱ��ȭ
	FGauge			m_fEnemyDetected_Gauge;		// �ִϸ��̼� ����� ����.

};

END