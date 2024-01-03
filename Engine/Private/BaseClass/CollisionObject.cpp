#include "BaseClass/CollisionObject.h"

#include "Component/ColliderComponent.h"

CCollisionObject::CCollisionObject()
{
	FAILED_CHECK_RETURN(Add_Component(TEXT("ColliderComp"), m_pColliderComp = CColliderComponent::Create()), );
	int t = 0;
}

CCollisionObject::CCollisionObject(const CCollisionObject& rhs)
{
	NULL_CHECK(m_pColliderComp = DynCast<CColliderComponent*>(m_pColliderComp->Clone()));
}

HRESULT CCollisionObject::Initialize_Prototype()
{
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));

	return S_OK;
}

HRESULT CCollisionObject::Initialize(void* Arg)
{
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));

	return S_OK;
}

void CCollisionObject::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CCollisionObject::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

}

void CCollisionObject::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);
}

HRESULT CCollisionObject::Render()
{

	return S_OK;
}


void CCollisionObject::Free()
{
	SUPER::Free();
}
