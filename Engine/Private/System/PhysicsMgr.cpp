#include "System/PhysicsMgr.h"

#include "BaseClass/GameObject.h"

// 성희 추가 Ray에 사용하는 매크로
#define CMP(x, y) \
      (fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

CPhysicsMgr::CPhysicsMgr()
{

}

HRESULT CPhysicsMgr::Initialize(const _uint iMaxPhysicsWorld3D)
{
	m_vecWorld3D.reserve(iMaxPhysicsWorld3D);
	m_vecWorld3D.clear();

	for (_uint i = 0; i < m_vecWorld3D.capacity(); i++)
	{
		m_vecWorld3D.push_back(CPhysicsWorld3D::Create(1000, 1000));
	}

	return S_OK;
}

void CPhysicsMgr::StartFrame()
{
	for (auto iter = m_vecWorld3D.begin(); iter != m_vecWorld3D.end(); ++iter)
	{
		(*iter)->StartFrame_Physics();
	}
}

_int CPhysicsMgr::Tick(const _float& fTimeDelta)
{
	for (auto iter = m_vecWorld3D.begin(); iter != m_vecWorld3D.end(); ++iter)
	{
		(*iter)->Update_Physics(fTimeDelta);
	}

	return 0;
}

CPhysicsMgr* CPhysicsMgr::Create(const _uint iMaxPhysicsWorld3D)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(iMaxPhysicsWorld3D)))
	{
		MSG_BOX("PhysicsMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CPhysicsMgr::Free()
{
	for (auto item : m_vecWorld3D)
	{
		Safe_Release(item);
	}
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_GetGameObject(const _uint iWorldID, FCollisionPrimitive* pSrc)
{
	list_collide_test listCollision;
	list<pair<CGameObject*, FContact>> listGameObject;

	listCollision = m_vecWorld3D[iWorldID]->Test_Contacts(Cast<FCollisionPrimitive*>(pSrc));
	for (auto iter = listCollision.begin(); iter != listCollision.end(); ++iter)
	{
		CColliderComponent* pCollideComp = Cast<CColliderComponent*>((*iter).first->Get_Owner());
		CGameObject* pObj = Cast<CGameObject*>(pCollideComp->Get_OwnerObject());
		listGameObject.push_back(pair<CGameObject*, FContact>(pObj, (*iter).second));
	}

	return listGameObject;
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Collider_GetGameObject(const _uint iWorldID, const _float3 vPos, CColliderComponent* pSrc, _ulong iMask)
{
	FCollisionPrimitive* pShape = static_cast<FCollisionPrimitive*>(pSrc->Get_Shape());
	_float3 vSetPos = {};
	XMStoreFloat3(&vSetPos, pShape->Get_PositionVector() + XMLoadFloat3(&vPos));
	pShape->Set_Position(vSetPos);
	pShape->Set_CollisionMask(iMask);

	return IntersectTests_GetGameObject(iWorldID, pShape);
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Sphere_GetGameObject(const _uint iWorldID, _float3 vPos, _float fRadius, _ulong iMask)
{
	FCollisionSphere pShape;
	pShape.Set_Position(_float3(vPos.x, vPos.y, vPos.z));
	pShape.fBaseRadius = fRadius;
	pShape.Set_CollisionMask(iMask);
	pShape.pBody->CalculateDerivedData();
	pShape.Calculate_Transform();
	pShape.Calculate_Shape();

	return IntersectTests_GetGameObject(iWorldID, static_cast<FCollisionPrimitive*>(&pShape));
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Box_GetGameObject(const _uint iWorldID, _float3 vPos, _float3 vHalfSize, _ulong iMask)
{
	FCollisionBox pShape;
	pShape.Set_Position(_float3(vPos.x, vPos.y, vPos.z));
	pShape.vHalfSize = _float3(vHalfSize.x, vHalfSize.y, vHalfSize.z);
	pShape.Set_CollisionMask(iMask);
	pShape.Calculate_Shape();
	pShape.Calculate_Transform();

	return IntersectTests_GetGameObject(iWorldID, static_cast<FCollisionPrimitive*>(&pShape));
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Capsule_GetGameObject(const _uint iWorldID, _float3 vPos, _float3 vNormal, _float fRadius, _ulong iMask)
{
	FCollisionCapsule pShape;
	pShape.Set_Position(_float3(vPos.x, vPos.y, vPos.z));
	pShape.vDirHalfSize = _float3(vNormal.x, vNormal.y, vNormal.z);
	pShape.fRadius = fRadius;
	pShape.Set_CollisionMask(iMask);
	pShape.Calculate_Shape();
	pShape.Calculate_Transform();

	return IntersectTests_GetGameObject(iWorldID, static_cast<FCollisionPrimitive*>(&pShape));
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Line_GetGameObject(const _uint iWorldID, _float3 vStart, _float3 vEnd, _ulong iMask)
{
	FCollisionLine pShape;
	//pShape.Set_Position(FVector3(vPos.x, vPos.y, vPos.z));
	pShape.vStart = _float3(vStart.x, vStart.y, vStart.z);
	pShape.vEnd = _float3(vEnd.x, vEnd.y, vEnd.z);
	pShape.Set_CollisionMask(iMask);
	pShape.Calculate_Shape();
	pShape.Calculate_Transform();

	return IntersectTests_GetGameObject(iWorldID, static_cast<FCollisionPrimitive*>(&pShape));
}

list<pair<CGameObject*, FContact>> CPhysicsMgr::IntersectTests_Ray_GetGameObject(const _uint iWorldID, const _float3 vPos, const _float3 vNormal, _ulong iMask)
{
	FCollisionRay pShape;
	pShape.vOrigin = (_float3(vPos.x, vPos.y, vPos.z));
	XMStoreFloat3(&pShape.vDir, XMVector3Normalize(XMLoadFloat3(&vNormal)));
	pShape.Set_CollisionMask(iMask);
	pShape.Calculate_Shape();
	pShape.Calculate_Transform();

	return IntersectTests_GetGameObject(iWorldID, static_cast<FCollisionPrimitive*>(&pShape));
}
