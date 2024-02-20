#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/PlaneModelComp.h"
#include "Utility/LogicDeviceBasic.h"


BEGIN(Client)

typedef FastDelegate0<void> StartDelegate;
typedef FastDelegate0<void> EndDelegate;

class CUI_FadeOut : public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_FadeOut)

protected:
	explicit CUI_FadeOut();
	explicit CUI_FadeOut(const CUI_FadeOut& rhs);
	virtual ~CUI_FadeOut() = default;

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
	static CUI_FadeOut* Create();
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

public:
	void Add_Event(StartDelegate StartEvent, EndDelegate EndEvent, const wstring& strReservedLevelTag);

private:
	StartDelegate		m_StartEvent = nullptr;
	EndDelegate			m_EndEvent = nullptr;

	wstring				m_strNextLevelTag = TEXT("");

private:
	FGauge			m_fUVSize = FGauge(1.f);
};

END