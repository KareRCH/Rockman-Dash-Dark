#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"

#include "GameObject/GameObjectFactory.h"

BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

/// <summary>
/// �浹ü�� �������� ������ �ʴ� ��ü, �������� ���δ�.
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
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

protected:
	HRESULT Initialize_Component();
	HRESULT Initialize_Component(FSerialData& InputData);


protected: // �浹
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