#include "Physics/BVHNode.h"

#include "Physics/RigidBody.h"

FBVHNode::~FBVHNode()
{
    // �θ��尡 ������ ���� ����� ����� �ǳʶڴ�.
    if (nullptr != pParent)
    {
        // Find our sibling
        FBVHNode* pSibling = { nullptr };
        if (pParent->pChildren[0] == this) 
            pSibling = pParent->pChildren[1];
        else 
            pSibling = pParent->pChildren[0];

        // �θ��忡 ���� ����� ���� ���� �����.
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

        // �̻��� ���� ���� ���� �ʿ����.
        pSibling->pParent = nullptr;
        pSibling->pBody = nullptr;
        pSibling->pChildren[0] = nullptr;
        pSibling->pChildren[1] = nullptr;
        Safe_Delete(pSibling);

        // Recalculate the parent's bounding volume
        pParent->RecalculateBoundingVolume();
    }

    // ���� ��忡 ���� ó���� ���� �ʰ� �ϸ鼭 ���� ��Ű��
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
    // �����̰ų� ����Ʈ�� 0�̸� 0�� ��ȯ
    if (IsLeaf() || iLimit == 0) return 0;

    // �������� �浹ü�� ����� ã�Ƴ���.
    return pChildren[0]->Get_PotentialContactsWith(
        pChildren[1], pContacts, iLimit
    );
}

void FBVHNode::Insert(FRigidBody* pNewBody, const FBoundingBox& NewVolume)
{
    // �������� Ȯ��
    if (IsLeaf())
    {
        if (pBody != nullptr && pBody->Get_Owner() != nullptr)
            Cast<FCollisionPrimitive*>(pBody->Get_Owner())->Remove_TransformChangedEvent();

        // ���� �ڽ����� ���� �ڽ��� ���� ī���Ͽ� ����ִ´�.
        pChildren[0] = new FBVHNode(
            this, Volume, pBody
        );

        // ������ �ڽ����� ���ο� ��带 �����.
        pChildren[1] = new FBVHNode(
            this, NewVolume, pNewBody
        );

        // ���� ���� Internal�� �����.
        this->pBody = nullptr;
        

        // �ٿ�� ������ �����Ѵ�.
        RecalculateBoundingVolume();
    }

    // Internal�̸� �� �ڽĵ� �� �ϳ��� �����ֱ� �Ѵ�.
    else
    {
        // ������ ũ�⸦ �յ��ϰ� �ϱ� ���� �ٿ�� �ڽ��� ������ ��ȭ���� ���� �й��Ѵ�.
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
    // Body�� �˻��ϰ� Delete�Ѵ�.
    FBVHNode* pNode = Find(_pBody);
    if (nullptr != pNode)
    {
        Safe_Delete(pNode);
    }
}

FBVHNode* FBVHNode::Find(FRigidBody* _pBody)
{
    // ������ �� ã�� �ٵ� ������ �� �ٵ� ��ȯ�Ѵ�.
    if (IsLeaf())
    {
        if (pBody == _pBody)
            return this;
    }
    else
    {
        // �����ȿ� ���ԵǸ� �ڽ��� ��ȸ�ϸ� ã�´�.
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
    // ����ó��
    if (!IsLeaf())
        return;

    //// �θ� ���� ��, ������ �˻縦 �Ѵ�.
    //if (pParent != nullptr)
    //{
    //    // ���� ������ ��ġ�°�?
    //    if (Overlaps(pParent))
    //    {
    //        // ������ ���� ��, �� ������ ��ģ�ٸ� ��� �����ϰ� �ٽ� �ֵ��� �Ѵ�.
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
    //        // �θ��� ������ ���ٸ�, �ٷ� �ٿ�� ���� ����
    //        else
    //        {
    //            RecalculateBoundingVolume();
    //        }
    //    }
    //    // ��ġ�� ������ �ٷ� ���ٰ� �ٽ� ����ִ´�.
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
    // BVH���� ��ġ�� �ʰų� ������ 0�̵Ǹ� �浹 ����� ���� �ʴ´�.
    if (!Overlaps(pOther) || iLimit == 0) return 0;

    // ��尣�� ������� �浹ó���� ���� ����Ʈ�� �ٵ� �ִ´�.
    if (IsLeaf() && pOther->IsLeaf())
    {
        pContacts->listBodies.push_back({ pBody, pOther->pBody });
        return 1;
    }

    // �����ų�
    // ���� ����� �� ũ��
    if (pOther->IsLeaf() ||
        (!IsLeaf() && Volume.Get_Size() >= pOther->Volume.Get_Size()))
    {
        // ��͸� ���� ���� ���
        _uint iCount = pChildren[0]->Get_PotentialContactsWith(
            pOther, pContacts, iLimit
        );

        // �浹���� ����Ʈ
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

    // �ٿ�� �ڽ� �ΰ��� �־� �ΰ��� ��ģ �������� �ٿ�� �ڽ��� ����� ����.
    Volume = FBoundingBox(
        pChildren[0]->Volume,
        pChildren[1]->Volume
    );

    // �θ� ��带 ��ġ�� �ٿ�� ������ ���
    if (pParent) 
        pParent->RecalculateBoundingVolume(true);
}

FBVHNode* FBVHNode::Get_Root()
{
    if (pParent == nullptr)
        return this;

    return pParent->Get_Root();
}
