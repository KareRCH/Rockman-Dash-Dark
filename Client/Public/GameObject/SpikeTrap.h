#pragma once


#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"
#include "GameObject/GameObjectFactory.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)
class CCommonModelComp;
END


BEGIN(Client)

class CSpikeTrap : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CSpikeTrap)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::SpikeTrap);

protected:
	explicit CSpikeTrap();
	explicit CSpikeTrap(const CSpikeTrap& rhs);
	virtual ~CSpikeTrap() = default;

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
	static CSpikeTrap* Create();
	static CSpikeTrap* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData_Prototype() override;
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();
	HRESULT	Initialize_Component(FSerialData& InputData);

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	FGauge		m_fIdleTime = FGauge(1.f);

public:
	enum class EState_Act { Idle, Attack, AttackEnd };

	void Register_State();

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Attack(const _float& fTimeDelta);
	void ActState_AttackEnd(const _float& fTimeDelta);

};

template <>
struct TObjectExtTrait<CSpikeTrap::g_ClassID>
{
	using Class = CSpikeTrap;
};

END