#pragma once

#include "Engine_Define.h"
#include "Component/SceneComponent.h"

#include "Physics/PhysicsWorld3D.h"
#include "Physics/Contact.h"

BEGIN(Engine)

class FCollisionPrimitive;

/// <summary>
/// �ݶ��̴� ������Ʈ�� �浹ü ������ Ʈ������ ������ �����ϴ� Ŭ�����Դϴ�.
/// </summary>
class ENGINE_DLL CColliderComponent : public CSceneComponent
{
	DERIVED_CLASS(CSceneComponent, CColliderComponent)

protected:
	explicit CColliderComponent();
	explicit CColliderComponent(const CColliderComponent& rhs);
	virtual ~CColliderComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	PRIVATE virtual HRESULT Initialize(void* Arg = nullptr) { return S_OK; }
	PUBLIC	virtual HRESULT Initialize(ECOLLISION eType);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) {}
	virtual HRESULT	Render() { return S_OK; }

public:
	static	CColliderComponent*		Create(ECOLLISION eType);
	virtual CComponent*				Clone(void* Arg = nullptr) override;

protected:
	virtual void				Free();

public:
	virtual void	EnterToPhysics(_uint iIndex);
	virtual void	ExitFromPhysics(_uint iIndex);
	// �Ϲ� ������Ʈ�� ���� ������ �ҷ����� ���� ������Ʈ �Լ�
	virtual void	Update_Physics(_matrix& matWorld);

public:		// �浹ü ����� ������
	GETSET_2(FCollisionPrimitive*, m_pCollisionShape, Shape, GET, SET)
	
protected:
	FCollisionPrimitive* m_pCollisionShape = nullptr;			// �浹ü ���� ������

public:			// �̺�Ʈ �Լ�
	template <typename Class>
	HRESULT	Set_Collision_Event(Class* pOwner, function<void(Class*, CGameObject*, const FContact* const)> fn);

	template <typename Class>
	HRESULT	Set_CollisionEntered_Event(Class* pOwner, function<void(Class*, CGameObject*, const FContact* const)> fn);

	template <typename Class>
	HRESULT	Set_CollisionExited_Event(Class* pOwner, function<void(Class*, CGameObject*)> fn);

protected:
	function<void(CGameObject*, const FContact* const)>		m_fnCollision;
	function<void(CGameObject*, const FContact* const)>		m_fnCollisionEntered;
	function<void(CGameObject*)>							m_fnCollisionExited;

protected:
	// �浹�� �߻��� �� �ҷ����� �Լ�. �浹�� �߻��ϸ� ����� �Լ��� �ٽ� ��ȣ�� �����ݴϴ�.
	// �浹��
	virtual void OnCollision(CColliderComponent* pDst, const FContact* const pContact);
	// �浹 ����, Collide�� �Բ� �ߵ�
	virtual void OnCollisionEntered(CColliderComponent* pDst, const FContact* const pContact);
	// �浹 ����, ��� �浹 üũ�� ������ ������ �ߵ�
	virtual void OnCollisionExited();

protected:
	using pair_collider = pair<CColliderComponent*, _bool>;
	// �浹 �߻��� �浹�� ��ü�� ����Ű�� ���� ����Ʈ, Collide�� ���� �� ������ �����˴ϴ�.
	list<pair_collider> m_listColliderObject;

public:
	GETSET_1(_uint, m_iCollisionLayer_Flag, CollisionLayer, GET_C_REF)
	void Set_CollisionLayer(const _uint iLayer_Flag)
	{
		m_iCollisionLayer_Flag = iLayer_Flag;
		if (m_pCollisionShape)
			m_pCollisionShape->Set_CollisionLayer(m_iCollisionLayer_Flag);
	}
	GETSET_1(_uint, m_iCollisionMask_Flag, CollisionMask, GET_C_REF)
	void Set_CollisionMask(const _uint iMask_Flag)
	{
		m_iCollisionMask_Flag = iMask_Flag;
		if (m_pCollisionShape)
			m_pCollisionShape->Set_CollisionMask(m_iCollisionMask_Flag);
	}

protected:
	_uint				m_iCollisionLayer_Flag;			// �ݸ��� ���̾�, �浹ü�� �����ϴ� ��
	_uint				m_iCollisionMask_Flag;
	// �ݸ��� ����ũ, �浹ü�� �浹�ϰ� �;��ϴ� ��
};

END


template <typename Class>
inline HRESULT	CColliderComponent::Set_Collision_Event(Class* pOwner, function<void(Class*, CGameObject*, const FContact* const)> fn)
{
	m_fnCollision = [pOwner, fn](CGameObject* pDst, const FContact* const pContact) { fn(pOwner, pDst, pContact); };

	return S_OK;
}

template <typename Class>
inline HRESULT CColliderComponent::Set_CollisionEntered_Event(Class* pOwner, function<void(Class*, CGameObject*, const FContact* const)> fn)
{
	m_fnCollisionEntered = [pOwner, fn](CGameObject* pDst, const FContact* const pContact) { fn(pOwner, pDst, pContact); };

	return S_OK;
}

template <typename Class>
inline HRESULT CColliderComponent::Set_CollisionExited_Event(Class* pOwner, function<void(Class*, CGameObject*)> fn)
{
	m_fnCollisionExited = [pOwner, fn](CGameObject* pDst) { fn(pOwner, pDst); };

	return S_OK;
}
