#pragma once

#include "Engine_Define.h"
#include "Physics/CollisionPrimitive.h"

BEGIN(Engine)
class FRigidBody;

struct FPotentialContact
{
    /**
     * Holds the bodies that might be in contact.
     */
    list<pair<FRigidBody*, FRigidBody*>> listBodies;
};

class FBVHNode
{
public:
    FBVHNode(FBVHNode* pParent, const FBoundingBox& Volume, FRigidBody* pBody = nullptr)
        : pParent(pParent), Volume(Volume), pBody(pBody)
    {

    }
    ~FBVHNode();

public:
    FBVHNode* pChildren[2] = { nullptr, nullptr };

    FBoundingBox Volume;

    FRigidBody* pBody = { nullptr };

    FBVHNode* pParent = { nullptr };

public:
    // 리프노드 확인
    _bool IsLeaf() const { return (pBody != nullptr) || (pChildren[0] == nullptr) && (pChildren[1] == nullptr); }
    // 잠재적인 충돌체 개수 얻어내기
    _uint Get_PotentialContacts(FPotentialContact* pContacts, _uint iLimit) const;
    void Insert(FRigidBody* pBody, const FBoundingBox& Volume);
    void Remove(FRigidBody* pBody);
    FBVHNode* Find(FRigidBody* pBody);
    

protected:
    _bool Overlaps(const FBVHNode* pOther) const;
    _uint Get_PotentialContactsWith(const FBVHNode* pOther, FPotentialContact* pContacts, _uint iLimit) const;
    void RecalculateBoundingVolume(_bool bIsRecurse = true);
    FBVHNode* Get_Root();
};

END