#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"

#include "GameObject/GameObjectFactory.h"

BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

/// <summary>
/// 충돌체를 가지지만 보이지 않는 물체, 툴에서는 보인다.
/// </summary>
class CInvisibleObject : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CInvisibleObject)

public:
	static const _uint g_ClassID = ECast(EObjectIDExt::InvisibleObject);

protected:
	explicit CInvisibleObject();
	explicit CInvisibleObject(const CInvisibleObject& rhs);
	virtual ~CInvisibleObject() = default;

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
	static CInvisibleObject* Create();
	static CInvisibleObject* Create(FSerialData& InputData);
	virtual CGameObject* Clone(void* Arg = nullptr);
	virtual CGameObject* Clone(FSerialData& InputData);

protected:
	virtual void			Free() override;

protected:
protected:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

protected:
	HRESULT Initialize_Component();
	HRESULT Initialize_Component(FSerialData& InputData);


protected: // 충돌
	virtual void	OnCollision(CGameObject* pDst, const FContact* const pContact);
	virtual void	OnCollisionEntered(CGameObject* pDst, const FContact* const pContact);
	virtual void	OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };
};

template <>
struct TObjectExtTrait<CInvisibleObject::g_ClassID>
{
	using Class = CInvisibleObject;
};

END