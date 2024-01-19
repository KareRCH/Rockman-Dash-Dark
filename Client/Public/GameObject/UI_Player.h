#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Utility/LogicDeviceBasic.h"
#include "CloudStation/CloudStation_Player.h"

BEGIN(Engine)
class CCloudStationComp;
class CPlaneModelComp;
END

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

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();

public:
	void Update_CloudStation();

private:
	CCloudStationComp* m_pCloudStationComp = { nullptr };
	CCloudStation_Player* m_pPlayerCloud = { nullptr };

private:
	_bool				m_bIsDisplay = { false };
	FGauge				m_fDisplayLerp = FGauge(1.f);

public:
	enum TYPE { HP_CONTAINER, HP_FRONT, SUB_CONTAINER, SUB_USAGE, SUB_MAIN, THROW, ENEMY_SPOT, TYPE_END };

private:
	CPlaneModelComp*	m_pImage[TYPE_END] = {};
	_float3				m_vOriginPos[TYPE_END] = {};

	FGauge				m_fHP;						// HP
	FGauge				m_fHP_Delayed;				// HP 지연된 값
	FDelay				m_fHP_ReduceTime;			// HP 깎기 지연시간

private:
	FGauge				m_fSubWeapon_Main;			// 실제 잔탄량
	FGauge				m_fSubWeapon_Usage;			// 현재 사용 가능한 잔탄
	FGauge				m_fSubWeapon_Usage_Delayed;	// 현재 사용 가능 잔탄 딜레이 처리

private:
	_bool				m_bIsEnemySpoted = { false };		// 시야 밖에서 적이 범위 안에 있을 때 활성화
	FGauge				m_fEnemySpot_Index = FGauge(12.f);	// 애니메이션 재생시 사용됨.

	

};

END