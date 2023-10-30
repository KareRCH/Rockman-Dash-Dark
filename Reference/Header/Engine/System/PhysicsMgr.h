#pragma once

#include "Base.h"

#include "Physics/PhysicsWorld3D.h"
#include "Component/ColliderComponent.h"


BEGIN(Engine)

class CGameObject;

/// <summary>
/// ���� ���踦 �����ϴ� Ŭ����
/// </summary>
class CPhysicsMgr final : public CBase
{
	DERIVED_CLASS(CBase, CPhysicsMgr)

private:
	explicit CPhysicsMgr();
	virtual ~CPhysicsMgr() = default;

public:
	HRESULT			Initialize(const _uint iMaxPhysicsWorld3D);
	void			StartFrame();
	// ���� ��Ȯ���� ���� Precision�� �Ǽ� Ÿ���� ����Ѵ�.
	_int			Tick(const Real& fTimeDelta);

public:
	static CPhysicsMgr* Create(const _uint iMaxPhysicsWorld3D);

private:
	virtual void	Free();

public:
	GETSET_1(vector<CPhysicsWorld3D*>, m_vecWorld3D, VecWorld3D, GET_PTR)
	CPhysicsWorld3D* Get_World3D(const _uint iWorldID) const { return m_vecWorld3D[iWorldID]; }

private:
	vector<CPhysicsWorld3D*> m_vecWorld3D;

public:	// �������迡 �浹 �׽�Ʈ�� ������ �� ����Ʈ�� �����ޱ�
	list<pair<CGameObject*, FContact>> IntersectTests_GetGameObject(const _uint iWorldID, FCollisionPrimitive* pSrc);

	list<pair<CGameObject*, FContact>> IntersectTests_Collider_GetGameObject(const _uint iWorldID, const _vec3 vPos, CColliderComponent* pSrc, _ulong iMask);
	list<pair<CGameObject*, FContact>> IntersectTests_Sphere_GetGameObject(const _uint iWorldID, _vec3 vPos, _float fRadius, _ulong iMask);
	list<pair<CGameObject*, FContact>> IntersectTests_Box_GetGameObject(const _uint iWorldID, _vec3 vPos, _vec3 vHalfSize, _ulong iMask);
	list<pair<CGameObject*, FContact>> IntersectTests_Capsule_GetGameObject(const _uint iWorldID, _vec3 vPos, _vec3 vNormal, _float fRadius, _ulong iMask);
	list<pair<CGameObject*, FContact>> IntersectTests_Line_GetGameObject(const _uint iWorldID, _vec3 vStart, _vec3 vEnd, _ulong iMask);
	list<pair<CGameObject*, FContact>> IntersectTests_Ray_GetGameObject(const _uint iWorldID, const _vec3 vPos, const _vec3 vNormal, _ulong iMask);
};

END