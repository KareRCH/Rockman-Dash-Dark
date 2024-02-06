#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/PlaneModelComp.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Client)

typedef FastDelegate0<void> StartDelegate;
typedef FastDelegate0<void> EndDelegate;

class CUI_Blink : public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_Blink)

protected:
	explicit CUI_Blink();
	explicit CUI_Blink(const CUI_Blink& rhs);
	virtual ~CUI_Blink() = default;

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
	static CUI_Blink* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	virtual FSerialData SerializeData() override;

public:
	HRESULT Initialize_Component();
	HRESULT Bind_ShaderResources();

private:
	class CPlaneModelComp* m_pPlaneModelComp = { nullptr };

public:
	enum class EState_Act { Idle, FadeOut };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_FadeOut(const _float& fTimeDelta);

private:
	_bool			m_bIsBlink = { false };
	FGauge			m_fBlink = FGauge(0.05f);
	FGauge			m_fAlpha = FGauge(1.f);
};

END