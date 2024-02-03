#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Utility/LogicDeviceBasic.h"
#include "CloudStation/CloudStation_Boss.h"

BEGIN(Engine)
class CCloudStationComp;
class CPlaneModelComp;
END




BEGIN(Client)

/// <summary>
/// º¸½º UI
/// </summary>
class CUI_Boss : public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_Boss)

protected:
	explicit CUI_Boss();
	explicit CUI_Boss(const CUI_Boss& rhs);
	virtual ~CUI_Boss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual void OnCreated() override;
	virtual void BeginPlay() override;

public:
	static CUI_Boss*	Create();
	CGameObject*		Clone(void* Arg = nullptr);

protected:
	virtual void		Free() override;

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT				Initialize_Component();

public:
	void				Update_CloudStation();

private:
	CCloudStationComp* m_pCloudStationComp = { nullptr };
	CCloudStation_Boss* m_pBossCloud = { nullptr };

private:
	_bool				m_bIsDisplay = { false };
	FGauge				m_fDisplayLerp = FGauge(1.f);

public:
	enum TYPE { HP_CONTAINER, HP_BAR, TYPE_END };

private:
	CPlaneModelComp*	m_pImage[TYPE_END] = {};
	_float3				m_vOriginPos[TYPE_END] = {};

	FGauge				m_fHP;						// HP

	FRoundTripGauge		m_fFontDisplayTime = FRoundTripGauge(0.f, 2.5f);
};

END