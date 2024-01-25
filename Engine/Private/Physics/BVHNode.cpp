#include "Physics/BVHNode.h"

#include "Physics/RigidBody.h"

FBVHNode::~FBVHNode()
{
    // 부모노드가 없으면 형제 노드의 계산을 건너뛴다.
    if (nullptr != pParent)
    {
        // Find our sibling
        FBVHNode* pSibling = { nullptr };
        if (pParent->pChildren[0] == this) 
            pSibling = pParent->pChildren[1];
        else 
            pSibling = pParent->pChildren[0];

        // 부모노드에 형제 노드의 값을 덮어 씌운다.
        pParent->Volume = pSibling->Volume;
        pParent->pBody = pSibling->pBody;
        pParent->pChildren[0] = pSibling->pChildren[0];
        pParent->pChildren[1] = pSibling->pChildren[1];

        if (pParent->pBody != nullptr && pParent->pBody->Get_Owner() != nullptr)
            Cast<FCollisionPrimitive*>(pParent->pBody->Get_Owner())->Remove_TransformChangedEvent();

        if (pSibling->pChildren[0] != nullptr)
            pSibling->pChildren[0]->pParent = pParent;
        if (pSibling->pChildren[1] != nullptr)
            pSibling->pChildren[1]->pParent = pParent;

        // 이사한 형제 노드는 이제 필요없다.
        pSibling->pParent = nullptr;
        pSibling->pBody = nullptr;
        pSibling->pChildren[0] = nullptr;
        pSibling->pChildren[1] = nullptr;
        Safe_Delete(pSibling);

        // Recalculate the parent's bounding volume
        pParent->RecalculateBoundingVolume();
    }

    // 형제 노드에 대한 처리를 하지 않게 하면서 삭제 시키기
    if (nullptr != pChildren[0])
    {
        pChildren[0]->pParent = nullptr;
        Safe_Delete(pChildren[0]);
        pChildren[0] = nullptr;
    }
    if (nullptr != pChildren[1])
    {
        pChildren[1]->pParent = nullptr;
        Safe_Delete(pChildren[1]);
        pChildren[1] = nullptr;
    }
}

_uint FBVHNode::Get_PotentialContacts(FPotentialContact* pContacts, _uint iLimit) const
{
    // 리프이거나 리미트가 0이면 0을 반환
    if (IsLeaf() || iLimit == 0) return 0;

    // 잠재적인 충돌체가 몇개인지 찾아낸다.
    return pChildren[0]->Get_PotentialContactsWith(
        pChildren[1], pContacts, iLimit
    );
}

void FBVHNode::Insert(FRigidBody* pNewBody, const FBoundingBox& NewVolume)
{
    // 리프인지 확인
    if (IsLeaf())
    {
        if (pBody != nullptr && pBody->Get_Owner() != nullptr)
            Cast<FCollisionPrimitive*>(pBody->Get_Owner())->Remove_TransformChangedEvent();

        // 왼쪽 자식으로 현재 자신의 것을 카피하여 집어넣는다.
        pChildren[0] = new FBVHNode(
            this, Volume, pBody
        );

        // 오른쪽 자식으로 새로운 노드를 만든다.
        pChildren[1] = new FBVHNode(
            this, NewVolume, pNewBody
        );

        // 기존 것을 Internal로 만든다.
        this->pBody = nullptr;
        

        // 바운딩 볼륨을 재계산한다.
        RecalculateBoundingVolume();
    }

    // Internal이면 두 자식들 중 하나에 끼워넣기 한다.
    else
    {
        // 볼륨의 크기를 균등하게 하기 위해 바운딩 박스의 사이즈 변화율에 따라 분배한다.
        if (pChildren[0]->Volume.Get_Growth(NewVolume) <
            pChildren[1]->Volume.Get_Growth(NewVolume))
        {
            pChildren[0]->Insert(pNewBody, NewVolume);
        }
        else
        {
            pChildren[1]->Insert(pNewBody, NewVolume);
        }
    }
}

void FBVHNode::Remove(FRigidBody* _pBody)
{
    // Body를 검색하고 Delete한다.
    FBVHNode* pNode = Find(_pBody);
    if (nullptr != pNode)
    {
        Safe_Delete(pNode);
    }
}

FBVHNode* FBVHNode::Find(FRigidBody* _pBody)
{
    // 리프일 때 찾는 바디가 있으면 그 바디를 반환한다.
    if (IsLeaf())
    {
        if (pBody == _pBody)
            return this;
    }
    else
    {
        // 볼륨안에 포함되면 자식을 순회하며 찾는다.
        FCollisionPrimitive* pCol = ReCast<FCollisionPrimitive*>(_pBody->Get_Owner());
        if (Volume.Overlaps(&pCol->BoundingBox))
        {
            FBVHNode* pNode1 = { nullptr };
            FBVHNode* pNode2 = { nullptr };

            if (pChildren[0] != nullptr)
                pNode1 = pChildren[0]->Find(_pBody);
            if (pChildren[1] != nullptr)
                pNode2 = pChildren[1]->Find(_pBody);

            if (nullptr != pNode1 && nullptr != pNode2)
                int t = 0;

            if (nullptr != pNode1)
                return pNode1;
            if (nullptr != pNode2)
                return pNode2;
        }
    }

    return nullptr;
}

void FBVHNode::BodyMoved()
{
    // 예외처리
    if (!IsLeaf())
        return;

    //// 부모가 있을 때, 오버랩 검사를 한다.
    //if (pParent != nullptr)
    //{
    //    // 현재 영역에 겹치는가?
    //    if (Overlaps(pParent))
    //    {
    //        // 삼촌이 있을 때, 그 영역과 겹친다면 노드 삭제하고 다시 넣도록 한다.
    //        if (pParent->pParent != nullptr)
    //        {
    //            FBVHNode* pParentSibling = { nullptr };
    //            if (pParent->pParent->pChildren[0] == pParent)
    //                pParentSibling = pParent->pParent->pChildren[1];
    //            else
    //                pParentSibling = pParent->pParent->pChildren[0];
    //            
    //            if (Overlaps(pParentSibling))
    //            {
    //                FRigidBody* pMovedBody = pBody;
    //                FBVHNode* pRoot = Get_Root();
    //                delete this;
    //                FCollisionPrimitive* pCol = ReCast<FCollisionPrimitive*>(pMovedBody->Get_Owner());
    //                pRoot->Insert(pMovedBody, pCol->BoundingBox);
    //            }
    //            else
    //            {
    //                RecalculateBoundingVolume();
    //            }
    //        }
    //        // 부모의 형제가 없다면, 바로 바운딩 볼륨 조절
    //        else
    //        {
    //            RecalculateBoundingVolume();
    //        }
    //    }
    //    // 겹치지 않으면 바로 뺐다가 다시 집어넣는다.
    //    else
    //    {
    //        FRigidBody* pMovedBody = pBody;
    //        FBVHNode* pRoot = Get_Root();
    //        delete this;
    //        FCollisionPrimitive* pCol = ReCast<FCollisionPrimitive*>(pMovedBody->Get_Owner());
    //        pRoot->Insert(pMovedBody, pCol->BoundingBox);
    //    }
    //}

    FRigidBody* pMovedBody = pBody;
    FBVHNode* pRoot = Get_Root();
    delete this;
    FCollisionPrimitive* pCol = ReCast<FCollisionPrimitive*>(pMovedBody->Get_Owner());
    pRoot->Insert(pMovedBody, pCol->BoundingBox);
}

_bool FBVHNode::Overlaps(const FBVHNode* pOther) const
{
    return Volume.Overlaps(&pOther->Volume);
}

_uint FBVHNode::Get_PotentialContactsWith(const FBVHNode* pOther, FPotentialContact* pContacts, _uint iLimit) const
{
    // BVH간에 겹치지 않거나 제한이 0이되면 충돌 계산을 하지 않는다.
    if (!Overlaps(pOther) || iLimit == 0) return 0;

    // 노드간에 리프라면 충돌처리를 위해 리스트에 바디를 넣는다.
    if (IsLeaf() && pOther->IsLeaf())
    {
        pContacts->listBodies.push_back({ pBody, pOther->pBody });
        return 1;
    }

    // 리프거나
    // 볼륨 사이즈가 더 크면
    if (pOther->IsLeaf() ||
        (!IsLeaf() && Volume.Get_Size() >= pOther->Volume.Get_Size()))
    {
        // 재귀를 거쳐 개수 계산
        _uint iCount = pChildren[0]->Get_PotentialContactsWith(
            pOther, pContacts, iLimit
        );

        // 충돌가능 리미트
        if (iLimit > iCount) 
        {
            return iCount + pChildren[1]->Get_PotentialContactsWith(
                pOther, pContacts, iLimit - iCount
            );
        }
        else 
        {
            return iCount;
        }
    }
    else
    {
        // Recurse into the other node
        _uint iCount = Get_PotentialContactsWith(
            pOther->pChildren[0], pContacts, iLimit
        );

        // Check we have enough slots to do the other side too
        if (iLimit > iCount) {
            return iCount + Get_PotentialContactsWith(
                pOther->pChildren[1], pContacts, iLimit - iCount
            );
        }
        else {
            return iCount;
        }
    }
}

void FBVHNode::RecalculateBoundingVolume(_bool bIsRecurse)
{
    if (IsLeaf()) return;

    // 바운딩 박스 두개를 넣어 두개를 합친 사이즈의 바운딩 박스를 만들어 낸다.
    Volume = FBoundingBox(
        pChildren[0]->Volume,
        pChildren[1]->Volume
    );

    // 부모 노드를 거치며 바운딩 볼륨을 계산
    if (pParent) 
        pParent->RecalculateBoundingVolume(true);
}

FBVHNode* FBVHNode::Get_Root()
{
    if (pParent == nullptr)
        return this;

    return pParent->Get_Root();
}
