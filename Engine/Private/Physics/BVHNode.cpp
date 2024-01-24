#include "Physics/BVHNode.h"

#include "Physics/RigidBody.h"

FBVHNode::~FBVHNode()
{
    // �θ��尡 ������ ���� ����� ����� �ǳʶڴ�.
    if (nullptr != pParent)
    {
        // Find our sibling
        FBVHNode* pSibling;
        if (pParent->pChildren[0] == this) 
            pSibling = pParent->pChildren[1];
        else 
            pSibling = pParent->pChildren[0];

        // Write its data to our parent
        pParent->Volume = pSibling->Volume;
        pParent->pBody = pSibling->pBody;
        pParent->pChildren[0] = pSibling->pChildren[0];
        pParent->pChildren[1] = pSibling->pChildren[1];

        // Delete the sibling (we blank its parent and
        // children to avoid processing/deleting them)
        pSibling->pParent = nullptr;
        pSibling->pBody = nullptr;
        pSibling->pChildren[0] = nullptr;
        pSibling->pChildren[1] = nullptr;
        Safe_Delete(pSibling);

        // Recalculate the parent's bounding volume
        pParent->RecalculateBoundingVolume();
    }

    // Delete our children (again we remove their
    // parent data so we don't try to process their siblings
    // as they are deleted).
    if (pChildren[0]) 
    {
        pChildren[0]->pParent = nullptr;
        Safe_Delete(pChildren[0]);
    }
    if (pChildren[1]) 
    {
        pChildren[1]->pParent = nullptr;
        Safe_Delete(pChildren[1]);
    }
}

_uint FBVHNode::Get_PotentialContacts(FPotentialContact* pContacts, _uint iLimit) const
{
    // �����̰ų� ����Ʈ�� 0�̸� 0�� ��ȯ
    if (IsLeaf() || iLimit == 0) return 0;

    if (pChildren[0] == nullptr || pChildren[1] == nullptr)
        return 0;

    // �������� �浹ü�� ����� ã�Ƴ���.
    return pChildren[0]->Get_PotentialContactsWith(
        pChildren[1], pContacts, iLimit
    );
}

void FBVHNode::Insert(FRigidBody* pNewBody, const FBoundingBox& NewVolume)
{
    // �������� Ȯ��
    if (IsLeaf() || pChildren[0] == nullptr && pChildren[1] == nullptr)
    {
        // Child one is a copy of us.
        pChildren[0] = new FBVHNode(
            this, Volume, pBody
        );

        // Child two holds the new body
        pChildren[1] = new FBVHNode(
            this, NewVolume, pNewBody
        );

        // And we now loose the body (we're no longer a leaf)
        this->pBody = nullptr;

        // We need to recalculate our bounding volume
        RecalculateBoundingVolume();
    }

    // Otherwise we need to work out which child gets to keep
    // the inserted body. We give it to whoever would grow the
    // least to incorporate it.
    else
    {
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
    FBVHNode* pNode = Find(_pBody);
    if (nullptr != pNode)
        Safe_Delete(pNode);
}

FBVHNode* FBVHNode::Find(FRigidBody* _pBody)
{
    // ������ �� �ٵ� ������ ����, ���� �Ǹ鼭 ���ÿ� �θ� ������ �ٿ�� �ڽ� ����
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

            if (nullptr != pNode1)
                return pNode1;
            if (nullptr != pNode2)
                return pNode2;
        }
    }

    return nullptr;
}

_bool FBVHNode::Overlaps(const FBVHNode* pOther) const
{
    return Volume.Overlaps(&pOther->Volume);
}

_uint FBVHNode::Get_PotentialContactsWith(const FBVHNode* pOther, FPotentialContact* pContacts, _uint iLimit) const
{
    // Early out if we don't overlap or if we have no room
        // to report contacts
    if (!Overlaps(pOther) || iLimit == 0) return 0;

    // If we're both at leaf nodes, then we have a potential contact
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
    if (pParent) pParent->RecalculateBoundingVolume(true);
}
