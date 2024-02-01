#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"
#include "GameObject/GameObjectFactory.h"



BEGIN(Client)

class CTrigger : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CTrigger)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::Trigger);

protected:
	explicit CTrigger();
	explicit CTrigger(const CTrigger& rhs);
	virtual ~CTrigger() = default;

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
	static CTrigger* Create();
	static CTrigger* Create(FSerialData& InputData);
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
};

template <>
struct TObjectExtTrait<CTrigger::g_ClassID>
{
	using Class = CTrigger;
};

END