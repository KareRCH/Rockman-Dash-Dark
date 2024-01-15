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
/// 콜라이더 컴포넌트는 충돌체 정보와 트랜스폼 정보를 저장하는 클래스입니다.
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
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	HRESULT Bind_Collision(ECollisionType eType);

public:
	// 물리세계에 Body 추가
	virtual void	EnterToPhysics(_uint iIndex);
	// 물리세계에서 Body 제거
	virtual void	ExitFromPhysics(_uint iIndex);
	// 일반 업데이트가 끝난 다음에 불러오는 물리 업데이트 함수
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


public:		// 충돌체 저장용 포인터
	GETSET_2(FCollisionPrimitive*, m_pCollisionShape, Shape, GET, SET)
	
protected:
	FCollisionPrimitive* m_pCollisionShape = { nullptr };	// 충돌체 저장 포인터



public:		// 이벤트 함수
	void	Set_Collision_Event(CollisionEnterDelegate Event) { m_Collision_Event = Event; }
	void	Set_CollisionEntered_Event(CollisionEnterDelegate Event) { m_CollisionEntered_Event = Event; }
	void	Set_CollisionExited_Event(CollisionExitDelegate Event) { m_CollisionExited_Event = Event; }

protected:		// 충돌이 발생할 때 불러오는 함수. 충돌이 발생하면 연결된 함수로 다시 신호를 보내줍니다.
	// 충돌중
	virtual void OnCollision(void* pDst, const FContact* pContact);
	// 충돌 진입, Collide와 함께 발동
	virtual void OnCollisionEntered(void* pDst, const FContact* pContact);
	// 충돌 끝남, 모든 충돌 체크가 끝나는 시점에 발동
	virtual void OnCollisionExited();

protected:
	CollisionEnterDelegate		m_Collision_Event;
	CollisionEnterDelegate		m_CollisionEntered_Event;
	CollisionExitDelegate		m_CollisionExited_Event;



protected:
	using pair_collider = pair<CColliderComponent*, _bool>;
	// 충돌 발생시 충돌한 객체를 가리키기 위한 리스트, Collide를 통해 매 프레임 조절됩니다.
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
	_uint	m_iCollisionLayer_Flag = { 0 };		// 콜리전 레이어, 충돌체가 존재하는 층
	_uint	m_iCollisionMask_Flag = { 0 };		// 콜리전 마스크, 충돌체가 충돌하고 싶어하는 층
	
};

END
