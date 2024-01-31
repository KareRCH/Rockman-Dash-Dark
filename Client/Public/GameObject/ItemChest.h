#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"
#include "GameObject/GameObjectFactory.h"
#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CCommonModelComp;
END



BEGIN(Client)

typedef FastDelegate0<void> OpenDelegate;
typedef FastDelegate2<_bool, EItemObtain, void> GiveItemDelegate;
typedef FastDelegate0<void> EndDelegate;

/// <summary>
/// 아이템을 주는 상자
/// </summary>
class CItemChest : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CItemChest)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::ItemChest);

protected:
	explicit CItemChest();
	explicit CItemChest(const CItemChest& rhs);
	virtual ~CItemChest() = default;

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
	static	CItemChest* Create();
	static	CItemChest* Create(FSerialData& InputData);
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
	enum class EActionKey : _uint { Open, Close, Size };

private:
	ACTION_SET<EActionKey>	m_ActionKey;

public:
	enum class EState_Act { Idle, Open, Close };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

	FGauge m_fItemGiveDelay = FGauge(1.f);
	FGauge m_fWalkAndSmash = FGauge(0.3f);

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Open(const _float& fTimeDelta);
	void ActState_Close(const _float& fTimeDelta);

public:
	void Open_Chest(GiveItemDelegate Event, EndDelegate EndEvent);
	void Close_Chest();
	void Give_Item();
	void NothingInChest();

private:
	EItemObtain			m_eItemObtain = { EItemObtain::Money };
	OpenDelegate		m_OpenEvent;
	GiveItemDelegate	m_GiveItemEvent;
	EndDelegate			m_EndEvent;

};

template <>
struct TObjectExtTrait<CItemChest::g_ClassID>
{
	using Class = CItemChest;
};

END