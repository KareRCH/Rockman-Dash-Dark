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
    // 충돌체 깊은 복사
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

    // 충돌체에 충돌 이벤트 추가하기
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

    // + 물리 세계에서 제거 요청 코드 필요
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

    // 충돌체에 오너 설정
    m_pCollisionShape->Set_Owner(this);

    // 충돌체에 충돌 이벤트 추가하기
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    return S_OK;
}

HRESULT CColliderComponent::Initialize(ECOLLISION eType)
{
    if (nullptr == m_pCollisionShape)
        return E_FAIL;

    // 충돌체에 오너 설정
    m_pCollisionShape->Set_Owner(this);

    // 충돌체에 충돌 이벤트 추가하기
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

    // Exited 초기화
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
    // 충돌체가 충돌을 진행해야하는 객체인지 확인
    CColliderComponent* pDstCollider = ReCast<CColliderComponent*>(pDst);
    pDstCollider = DynCast<CColliderComponent*>(pDstCollider);
    if (nullptr == pDstCollider)
        return;

    // 충돌 중이었던 객체가 있는지 확인
    auto iter = find_if(m_listColliderObject.begin(), m_listColliderObject.end(), 
        [&pDst](pair_collider& pSrc) {
            return pDst == pSrc.first;
        });

    // 충돌 진입 함수 작동
    if (iter == m_listColliderObject.end())
    {
        OnCollisionEntered(pDst, pContact);
        m_listColliderObject.push_back(pair_collider(pDstCollider, true));
    }
    else
        iter->second = true;
    
    // CollideEvent 발동
    if (!m_Collision_Event.empty())
    {
        // 오너 객체가 있어야 해당 객체를 주인에게 넘겨준다.
        if (CGameObject* pObj = pDstCollider->Get_OwnerObject())
        {
            m_Collision_Event(pObj, pContact);
        }
    }
        
}

void CColliderComponent::OnCollisionEntered(void* pDst, const FContact* const pContact)
{
    // 충돌체가 충돌을 진행해야하는 객체인지 확인
    CColliderComponent* pDstCollider = ReCast<CColliderComponent*>(pDst);
    pDstCollider = DynCast<CColliderComponent*>(pDstCollider);
    if (nullptr == pDstCollider)
        return;

    // CollisionEntered 발동
    if (!m_CollisionEntered_Event.empty())
    {
        // 오너 객체가 있어야 해당 객체를 주인에게 넘겨준다.
        if (CGameObject* pObj = pDstCollider->Get_OwnerObject())
        {
            m_CollisionEntered_Event(pObj, pContact);
        }
    }
}

void CColliderComponent::OnCollisionExited()
{
    // 가지고 있던 충돌 정보를 확인하고
    for (auto iter = m_listColliderObject.begin(); iter != m_listColliderObject.end();)
    {
        if (!iter->second)
        {
            // 각 충돌제거시 이벤트에 대한 CollisionExited 발동
            if (!m_CollisionExited_Event.empty())
            {
                // 오너 객체가 있어야 해당 객체를 주인에게 넘겨준다.
                if (CGameObject* pObj = (*iter).first->Get_OwnerObject())
                {
                    m_CollisionExited_Event(pObj);
                }
            }
            iter = m_listColliderObject.erase(iter); // 계속해서 Exited를 돌지않게 충돌할경우 erase로 지우도록함  
        }
        else
            ++iter;
    }
}