#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

#include "Component/PlaneModelComp.h"
#include "Utility/LogicDeviceBasic.h"


BEGIN(Client)

// 대화중 특정 이벤트 발생시
typedef FastDelegate0<void> OccurDelegate;
typedef FastDelegate0<void> EndDelegate;

class CUI_Dialog : public CGameObject
{
	DERIVED_CLASS(CGameObject, CUI_Dialog)

protected:
	explicit CUI_Dialog();
	explicit CUI_Dialog(const CUI_Dialog& rhs);
	virtual ~CUI_Dialog() = default;

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
	static CUI_Dialog* Create();
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
	enum class EActionKey : _uint { Accept, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Opening, Dialog, Closing };

	void Register_State();
	void Input_Key();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Opening(const _float& fTimeDelta);
	void ActState_Dialog(const _float& fTimeDelta);
	void ActState_Closing(const _float& fTimeDelta);

public:
	void Add_Dialog(const wstring& strDialog, OccurDelegate Event = nullptr);
	void Set_Delay(_float fDelay) { m_fDelay.Readjust(fDelay); }
	void Set_EndEvent(EndDelegate Event) { m_EndEvent = Event; }

private:
	using SPairDialog = pair<wstring, OccurDelegate>;
	list<SPairDialog>	m_Dialogs;
	_uint				m_iCurDialogIndex = { 0 };
	FGauge				m_fDelay = FGauge(0.08f);
	OccurDelegate		m_OccurEvent = nullptr;
	EndDelegate			m_EndEvent = nullptr;

private:
	_float3 m_vOffset = { 0.f, -150.f, 0.f };

	_float3 m_vPos[9] = {};
	_float3 m_vScale[9] = {};
	FGauge	m_fLerp = FGauge(1.f);

};

END