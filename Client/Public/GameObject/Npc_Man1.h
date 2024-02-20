#pragma once

#include "Client_Define.h"
#include "GameObject/Npc_Common.h"
#include "GameObject/GameObjectFactory.h"
#include "Utility/DelegateTemplate.h"



BEGIN(Engine)
class CCommonModelComp;
END



BEGIN(Client)

typedef FastDelegate0<void> DialogStartDelegate;
typedef FastDelegate0<void> DialogEndDelegate;

/// <summary>
/// 대화를 하는 남성 NPC
/// </summary>
class CNpc_Man1 : public CNpc_Common
{
	DERIVED_CLASS(CCharacter_Common, CNpc_Man1)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::NpcMan1);

protected:
	explicit CNpc_Man1();
	explicit CNpc_Man1(const CNpc_Man1& rhs);
	virtual ~CNpc_Man1() = default;

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
	virtual void OnCreated() override;
	virtual void BeginPlay() override;

public:
	static	CNpc_Man1* Create();
	static	CNpc_Man1* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();
	// 시리얼 데이터로 초기화시 사용
	HRESULT	Initialize_Component(FSerialData& InputData);

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	enum class EActionKey : _uint { Dialog, EndDialog, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Idle, Dialog, EndDialog };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Dialog(const _float& fTimeDelta);
	void ActState_EndDialog(const _float& fTimeDelta);

public:
	_bool DialogStart(DialogStartDelegate Event, DialogEndDelegate EndEvent);
	void DialogEnd();

private:
	DialogStartDelegate			m_DialogStartEvent;
	DialogEndDelegate			m_DialogEndEvent;
};

template <>
struct TObjectExtTrait<CNpc_Man1::g_ClassID>
{
	using Class = CNpc_Man1;
};

END