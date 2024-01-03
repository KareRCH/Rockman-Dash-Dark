#include "Component/ColliderComponent.h"

#include "Physics/CollisionPrimitive.h"
#include "Physics/Contact.h"

CColliderComponent::CColliderComponent()
{
}

CColliderComponent::CColliderComponent(const CColliderComponent& rhs)
    : Base(rhs)
    , m_iCollisionLayer_Flag(rhs.m_iCollisionLayer_Flag)
    , m_iCollisionMask_Flag(rhs.m_iCollisionMask_Flag)
{
    // �浹ü ���� ����
    switch (rhs.m_pCollisionShape->Get_Type())
    {
    default:
    case ECOLLISION::SPHERE:
    {
        FCollisionSphere* pShape = DynCast<FCollisionSphere*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionSphere(*pShape);
        break;
    }
    case ECOLLISION::BOX:
    {
        FCollisionBox* pShape = DynCast<FCollisionBox*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionBox(*pShape);
        break;
    }
    case ECOLLISION::CAPSULE:
    {
        FCollisionCapsule* pShape = DynCast<FCollisionCapsule*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionCapsule(*pShape);
        break;
    }
    case ECOLLISION::PLANE:
    {
        FCollisionPlane* pShape = DynCast<FCollisionPlane*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionPlane(*pShape);
        break;
    }
    case ECOLLISION::LINE:
    {
        FCollisionLine* pShape = DynCast<FCollisionLine*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionLine(*pShape);
        break;
    }
    case ECOLLISION::RAY:
    {
        FCollisionRay* pShape = DynCast<FCollisionRay*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionRay(*pShape);
        break;
    }
    case ECOLLISION::TRIANGLE:
    {
        FCollisionTriangle* pShape = DynCast<FCollisionTriangle*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionTriangle(*pShape);
        break;
    }
    case ECOLLISION::OBB:
    {
        FCollisionOBB* pShape = DynCast<FCollisionOBB*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionOBB(*pShape);
        break;
    }
    }

    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));
}

CColliderComponent* CColliderComponent::Create(ECOLLISION eType)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(eType)))
    {
        MSG_BOX("Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CColliderComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ColliderBufferComp Copy Failed");
        Engine::Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CColliderComponent::Free()
{
    SUPER::Free();

    // + ���� ���迡�� ���� ��û �ڵ� �ʿ�
    ExitFromPhysics(m_iPhysics3dWorld_Index);
    Safe_Delete(m_pCollisionShape);
}

HRESULT CColliderComponent::Initialize_Prototype(ECOLLISION eType)
{
    switch (eType)
    {
    case ECOLLISION::SPHERE:
        m_pCollisionShape = new FCollisionSphere();
        break;
    case ECOLLISION::BOX:
        m_pCollisionShape = new FCollisionBox();
        break;
    case ECOLLISION::CAPSULE:
        m_pCollisionShape = new FCollisionCapsule();
        break;
    case ECOLLISION::PLANE:
        m_pCollisionShape = new FCollisionPlane();
        break;
    case ECOLLISION::LINE:
        m_pCollisionShape = new FCollisionLine();
        break;
    case ECOLLISION::RAY:
        m_pCollisionShape = new FCollisionRay();
        break;
    case ECOLLISION::TRIANGLE:
        m_pCollisionShape = new FCollisionTriangle();
        break;
    case ECOLLISION::OBB:
        m_pCollisionShape = new FCollisionOBB();
        break;
    }

    // �浹ü�� ���� ����
    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    return S_OK;
}

HRESULT CColliderComponent::Initialize(ECOLLISION eType)
{
    if (nullptr == m_pCollisionShape)
        return E_FAIL;

    // �浹ü�� ���� ����
    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    return S_OK;
}

void CColliderComponent::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CColliderComponent::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    OnCollisionExited();

    // Exited �ʱ�ȭ
    for (auto iter = m_listColliderObject.begin(); iter != m_listColliderObject.end(); ++iter)
        iter->second = false;
}

void CColliderComponent::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CColliderComponent::Render()
{

    return S_OK;
}

void CColliderComponent::EnterToPhysics(_uint iIndex)
{
    //Engine::Add_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::ExitFromPhysics(_uint iIndex)
{
    //Engine::Delete_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::Update_Physics(_matrix& matWorld)
{
    //Transform().Calculate_TransformFromParent(matWorld);

    //m_pCollisionShape->matOffset.RecieveDXArray(reinterpret_cast<float*>(Transform().Get_TransformMatrix() * matWorld));
}

void CColliderComponent::OnCollision(void* pDst, const FContact* const pContact)
{
    // �浹ü�� �浹�� �����ؾ��ϴ� ��ü���� Ȯ��
    CColliderComponent* pDstCollider = ReCast<CColliderComponent*>(pDst);
    pDstCollider = DynCast<CColliderComponent*>(pDstCollider);
    if (nullptr == pDstCollider)
        return;

    // �浹 ���̾��� ��ü�� �ִ��� Ȯ��
    auto iter = find_if(m_listColliderObject.begin(), m_listColliderObject.end(), 
        [&pDst](pair_collider& pSrc) {
            return pDst == pSrc.first;
        });

    // �浹 ���� �Լ� �۵�
    if (iter == m_listColliderObject.end())
    {
        OnCollisionEntered(pDst, pContact);
        m_listColliderObject.push_back(pair_collider(pDstCollider, true));
    }
    else
        iter->second = true;
    
    // CollideEvent �ߵ�
    if (!m_Collision_Event.empty())
    {
        // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
        if (CGameObject* pObj = pDstCollider->Get_OwnerObject())
        {
            m_Collision_Event(pObj, pContact);
        }
    }
        
}

void CColliderComponent::OnCollisionEntered(void* pDst, const FContact* const pContact)
{
    // �浹ü�� �浹�� �����ؾ��ϴ� ��ü���� Ȯ��
    CColliderComponent* pDstCollider = ReCast<CColliderComponent*>(pDst);
    pDstCollider = DynCast<CColliderComponent*>(pDstCollider);
    if (nullptr == pDstCollider)
        return;

    // CollisionEntered �ߵ�
    if (!m_CollisionEntered_Event.empty())
    {
        // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
        if (CGameObject* pObj = pDstCollider->Get_OwnerObject())
        {
            m_CollisionEntered_Event(pObj, pContact);
        }
    }
}

void CColliderComponent::OnCollisionExited()
{
    // ������ �ִ� �浹 ������ Ȯ���ϰ�
    for (auto iter = m_listColliderObject.begin(); iter != m_listColliderObject.end();)
    {
        if (!iter->second)
        {
            // �� �浹���Ž� �̺�Ʈ�� ���� CollisionExited �ߵ�
            if (!m_CollisionExited_Event.empty())
            {
                // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
                if (CGameObject* pObj = (*iter).first->Get_OwnerObject())
                {
                    m_CollisionExited_Event(pObj);
                }
            }
            iter = m_listColliderObject.erase(iter); // ����ؼ� Exited�� �����ʰ� �浹�Ұ�� erase�� ���쵵����  
        }
        else
            ++iter;
    }
}