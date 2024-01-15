#pragma once

#include "Engine_Define.h"
#include "Component/SceneComponent.h"

#include "Physics/PhysicsWorld3D.h"
#include "Physics/Contact.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)

class FCollisionPrimitive;

typedef FastDelegate2<CGameObject*, const FContact*>	CollisionEnterDelegate;
typedef FastDelegate1<CGameObject*>						CollisionExitDelegate;

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
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override { return S_OK; }
	virtual HRESULT Initialize_Prototype(ECollisionType eType);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	PRIVATE virtual HRESULT Initialize(void* Arg = nullptr) { return S_OK; }
	PUBLIC	virtual HRESULT Initialize(ECollisionType eType);
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	static	CColliderComponent* Create();
	static	CColliderComponent*	Create(ECollisionType eType);
	static	CColliderComponent* Create(FSerialData& InputData);
	virtual CComponent*			Clone(void* Arg = nullptr) override;

protected:
	virtual void				Free();

public:
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
	virtual FSerialData SerializeData();

public:
	HRESULT Bind_Collision(ECollisionType eType);

public:
	// �������迡 Body �߰�
	virtual void	EnterToPhysics(_uint iIndex);
	// �������迡�� Body ����
	virtual void	ExitFromPhysics(_uint iIndex);
	// �Ϲ� ������Ʈ�� ���� ������ �ҷ����� ���� ������Ʈ �Լ�
	virtual void	Update_Physics();

protected:
	_uint		m_iPhysics3dWorld_Index = { 0 };


#ifdef _DEBUG
private:
	_bool										m_bIsCollision = { false };
	PrimitiveBatch<VertexPositionColor>*		m_pBatch = { nullptr };
	BasicEffect*								m_pEffect = { nullptr };
	ComPtr<ID3D11InputLayout>					m_pInputLayout = { nullptr };
#endif


public:		// �浹ü ����� ������
	GETSET_2(FCollisionPrimitive*, m_pCollisionShape, Shape, GET, SET)
	
protected:
	FCollisionPrimitive* m_pCollisionShape = { nullptr };	// �浹ü ���� ������



public:		// �̺�Ʈ �Լ�
	void	Set_Collision_Event(CollisionEnterDelegate Event) { m_Collision_Event = Event; }
	void	Set_CollisionEntered_Event(CollisionEnterDelegate Event) { m_CollisionEntered_Event = Event; }
	void	Set_CollisionExited_Event(CollisionExitDelegate Event) { m_CollisionExited_Event = Event; }

protected:		// �浹�� �߻��� �� �ҷ����� �Լ�. �浹�� �߻��ϸ� ����� �Լ��� �ٽ� ��ȣ�� �����ݴϴ�.
	// �浹��
	virtual void OnCollision(void* pDst, const FContact* pContact);
	// �浹 ����, Collide�� �Բ� �ߵ�
	virtual void OnCollisionEntered(void* pDst, const FContact* pContact);
	// �浹 ����, ��� �浹 üũ�� ������ ������ �ߵ�
	virtual void OnCollisionExited();

protected:
	CollisionEnterDelegate		m_Collision_Event;
	CollisionEnterDelegate		m_CollisionEntered_Event;
	CollisionExitDelegate		m_CollisionExited_Event;



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
	_uint	m_iCollisionLayer_Flag = { 0 };		// �ݸ��� ���̾�, �浹ü�� �����ϴ� ��
	_uint	m_iCollisionMask_Flag = { 0 };		// �ݸ��� ����ũ, �浹ü�� �浹�ϰ� �;��ϴ� ��
	
};

END
