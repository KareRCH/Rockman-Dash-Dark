#include "Component/ColliderComponent.h"

#include "Physics/CollisionPrimitive.h"
#include "Physics/Contact.h"

CColliderComponent::CColliderComponent(const DX11DEVICE_T tDevice)
    : Base(tDevice)
    , m_iCollisionLayer_Flag(m_iCollisionLayer_Flag)
    , m_iCollisionMask_Flag(m_iCollisionMask_Flag)
{
}

CColliderComponent::CColliderComponent(const CColliderComponent& rhs)
    : Base(rhs)
    , m_iCollisionLayer_Flag(rhs.m_iCollisionLayer_Flag)
    , m_iCollisionMask_Flag(rhs.m_iCollisionMask_Flag)
{
    // �浹ü ������
    switch (rhs.m_pCollisionShape->Get_Type())
    {
    default:
    case ECOLLISION::SPHERE:
    {
        FCollisionSphere* pShape = dynamic_cast<FCollisionSphere*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionSphere(*pShape);
        break;
    }
    case ECOLLISION::BOX:
    {
        FCollisionBox* pShape = dynamic_cast<FCollisionBox*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionBox(*pShape);
        break;
    }
    case ECOLLISION::CAPSULE:
    {
        FCollisionCapsule* pShape = dynamic_cast<FCollisionCapsule*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionCapsule(*pShape);
        break;
    }
    case ECOLLISION::PLANE:
    {
        FCollisionPlane* pShape = dynamic_cast<FCollisionPlane*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionPlane(*pShape);
        break;
    }
    case ECOLLISION::LINE:
    {
        FCollisionLine* pShape = dynamic_cast<FCollisionLine*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionLine(*pShape);
        break;
    }
    case ECOLLISION::RAY:
    {
        FCollisionRay* pShape = dynamic_cast<FCollisionRay*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionRay(*pShape);
        break;
    }
    case ECOLLISION::TRIANGLE:
    {
        FCollisionTriangle* pShape = dynamic_cast<FCollisionTriangle*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionTriangle(*pShape);
        break;
    }
    case ECOLLISION::OBB:
    {
        FCollisionOBB* pShape = dynamic_cast<FCollisionOBB*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionOBB(*pShape);
        break;
    }
    }

    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Add_CollisionEvent([this](void* pDst, const FContact* const pContact) {
        this->OnCollision(static_cast<CColliderComponent*>(pDst), pContact);
        });

    // �̺�Ʈ �Լ� Ŭ�� ����, �������� �ܺο��� �߰�
}

CPrimitiveComponent* CColliderComponent::Create(const DX11DEVICE_T tDevice, ECOLLISION eType)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize(eType)))
    {
        Safe_Release(pInstance);

        MSG_BOX("Player Create Failed");
        return nullptr;
    }

    return pInstance;
}

CPrimitiveComponent* CColliderComponent::Clone(void* Arg)
{
    return new ThisClass(*this);
}

void CColliderComponent::Free()
{
    SUPER::Free();

    // + ���� ���迡�� ���� ��û �ڵ� �ʿ�
    ExitFromPhysics(0);
    Safe_Delete(m_pCollisionShape);
}

HRESULT CColliderComponent::Initialize(ECOLLISION eType)
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
    m_pCollisionShape->Add_CollisionEvent([this](void* pDst, const FContact* const pContact) {
        this->OnCollision(static_cast<CColliderComponent*>(pDst), pContact);
        });

    return S_OK;
}

void CColliderComponent::PriorityTick()
{
}

_int CColliderComponent::Tick(const _float& fTimeDelta)
{
    OnCollisionExited();
    // Exited �ʱ�ȭ
    for (auto iter = m_listColliderObject.begin(); iter != m_listColliderObject.end(); ++iter)
        iter->second = false;

    return 0;
}

void CColliderComponent::EnterToPhysics(_uint iIndex)
{
    //Engine::Add_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::ExitFromPhysics(_uint iIndex)
{
    //Engine::Delete_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::Update_Physics(const _matrix& matWorld)
{
    Calculate_Transform();
    Calculate_TransformFromParent(matWorld);

    //m_pCollisionShape->matOffset.RecieveDXArray(reinterpret_cast<float*>(Get_Transform()));
}

void CColliderComponent::OnCollision(CColliderComponent* pDst, const FContact* const pContact)
{
    // �浹ü�� �浹�� �����ؾ��ϴ� ��ü���� Ȯ��

    // �浹 ���̾��� ��ü�� �ִ��� Ȯ��
    auto iter = find_if(m_listColliderObject.begin(), m_listColliderObject.end(), 
        [&pDst](pair_collider& pSrc) {
            return pDst == pSrc.first;
        });

    // �浹 ���� �Լ� �۵�
    if (iter == m_listColliderObject.end())
    {
        OnCollisionEntered(pDst, pContact);
        m_listColliderObject.push_back(pair_collider(pDst, true));
    }
    else
        iter->second = true;
    
    // CollideEvent �ߵ�
    if (m_fnCollision)
    {
        // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
        if (CGameObject* pObj = pDst->Get_Owner())
        {
            m_fnCollision(pObj, pContact);
        }
    }
        
}

void CColliderComponent::OnCollisionEntered(CColliderComponent* pDst, const FContact* const pContact)
{
    // CollisionEntered �ߵ�
    if (m_fnCollisionEntered)
    {
        // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
        if (CGameObject* pObj = pDst->Get_Owner())
        {
            m_fnCollisionEntered(pObj, pContact);
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
            if (m_fnCollisionExited)
            {
                // ���� ��ü�� �־�� �ش� ��ü�� ���ο��� �Ѱ��ش�.
                if (CGameObject* pObj = (*iter).first->Get_Owner())
                {
                    m_fnCollisionExited(pObj);
                }
            }
            iter = m_listColliderObject.erase(iter); // ����ؼ� Exited�� �����ʰ� �浹�Ұ�� erase�� ���쵵����  
        }
        else
            ++iter;
    }
}

