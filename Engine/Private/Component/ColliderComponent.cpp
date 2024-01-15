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
    case ECollisionType::Sphere:
    {
        FCollisionSphere* pShape = DynCast<FCollisionSphere*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionSphere(*pShape);
        break;
    }
    case ECollisionType::Box:
    {
        FCollisionBox* pShape = DynCast<FCollisionBox*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionBox(*pShape);
        break;
    }
    case ECollisionType::Capsule:
    {
        FCollisionCapsule* pShape = DynCast<FCollisionCapsule*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionCapsule(*pShape);
        break;
    }
    case ECollisionType::Plane:
    {
        FCollisionPlane* pShape = DynCast<FCollisionPlane*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionPlane(*pShape);
        break;
    }
    case ECollisionType::Line:
    {
        FCollisionLine* pShape = DynCast<FCollisionLine*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionLine(*pShape);
        break;
    }
    case ECollisionType::Ray:
    {
        FCollisionRay* pShape = DynCast<FCollisionRay*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionRay(*pShape);
        break;
    }
    case ECollisionType::Triangle:
    {
        FCollisionTriangle* pShape = DynCast<FCollisionTriangle*>(rhs.m_pCollisionShape);
        m_pCollisionShape = new FCollisionTriangle(*pShape);
        break;
    }
    case ECollisionType::OBB:
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

CColliderComponent* CColliderComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("ColliderComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CColliderComponent* CColliderComponent::Create(ECollisionType eType)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(eType)))
    {
        MSG_BOX("ColliderComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CColliderComponent* CColliderComponent::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("ColliderComponent Create Failed");
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

#ifdef _DEBUG
    if (!m_bIsCloned)
    {
        Safe_Delete(m_pBatch);
        Safe_Delete(m_pEffect);
    }
#endif
}

FSerialData CColliderComponent::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", 1);

    switch (m_pCollisionShape->Get_Type())
    {
    case ECollisionType::Sphere:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Sphere");
        break;
    case ECollisionType::Box:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Box");
        break;
    case ECollisionType::Capsule:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Capsule");
        break;
    case ECollisionType::Plane:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Plane");
        break;
    case ECollisionType::Line:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Line");
        break;
    case ECollisionType::Ray:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Ray");
        break;
    case ECollisionType::Triangle:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_Triangle");
        break;
    case ECollisionType::OBB:
        Data.Add_MemberString("ProtoName", "Prototype_Collider_OBB");
        break;
    }
    Data.Add_Member("CollisionType", ECast(m_pCollisionShape->Get_Type()));

    return Data;
}

FSerialData CColliderComponent::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", 1);
    Data.Add_MemberString("ProtoName", ConvertToString(m_strPrototypeName));

    Data.Add_Member("CollisionType", ECast(m_pCollisionShape->Get_Type()));
    Data.Add_Member("CollisionLayer", m_iCollisionLayer_Flag);
    Data.Add_Member("CollisionMask", m_iCollisionMask_Flag);

    return Data;
}

HRESULT CColliderComponent::Bind_Collision(ECollisionType eType)
{
    if (nullptr != m_pCollisionShape)
    {
        ExitFromPhysics(m_iPhysics3dWorld_Index);
        Safe_Delete(m_pCollisionShape);
    }

    switch (eType)
    {
    case ECollisionType::Sphere:
        m_pCollisionShape = new FCollisionSphere();
        break;
    case ECollisionType::Box:
        m_pCollisionShape = new FCollisionBox();
        break;
    case ECollisionType::Capsule:
        m_pCollisionShape = new FCollisionCapsule();
        break;
    case ECollisionType::Plane:
        m_pCollisionShape = new FCollisionPlane();
        break;
    case ECollisionType::Line:
        m_pCollisionShape = new FCollisionLine();
        break;
    case ECollisionType::Ray:
        m_pCollisionShape = new FCollisionRay();
        break;
    case ECollisionType::Triangle:
        m_pCollisionShape = new FCollisionTriangle();
        break;
    case ECollisionType::OBB:
        m_pCollisionShape = new FCollisionOBB();
        break;
    }

    // �浹ü�� ���� ����
    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    // 
    m_pCollisionShape->Set_TransformEvent(MakeDelegate(this, &ThisClass::Calculate_TransformMatrixFromParent));

    // ����׿� �ݸ��� �ð� ǥ��
#ifdef _DEBUG
    m_pBatch = new PrimitiveBatch<VertexPositionColor>(D3D11Context());
    m_pEffect = new BasicEffect(D3D11Device());
    m_pEffect->SetVertexColorEnabled(true);

    const void* pShaderByteCode = { nullptr };
    size_t	iShaderCodeLength = { 0 };

    m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderCodeLength);

    if (FAILED(D3D11Device()->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderCodeLength, m_pInputLayout.GetAddressOf())))
        return E_FAIL;

#endif

    return S_OK;
}

HRESULT CColliderComponent::Initialize_Prototype(ECollisionType eType)
{
    switch (eType)
    {
    case ECollisionType::Sphere:
        m_pCollisionShape = new FCollisionSphere();
        break;
    case ECollisionType::Box:
        m_pCollisionShape = new FCollisionBox();
        break;
    case ECollisionType::Capsule:
        m_pCollisionShape = new FCollisionCapsule();
        break;
    case ECollisionType::Plane:
        m_pCollisionShape = new FCollisionPlane();
        break;
    case ECollisionType::Line:
        m_pCollisionShape = new FCollisionLine();
        break;
    case ECollisionType::Ray:
        m_pCollisionShape = new FCollisionRay();
        break;
    case ECollisionType::Triangle:
        m_pCollisionShape = new FCollisionTriangle();
        break;
    case ECollisionType::OBB:
        m_pCollisionShape = new FCollisionOBB();
        break;
    }

    // �浹ü�� ���� ����
    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    return S_OK;
}

HRESULT CColliderComponent::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    _uint iCollisionType = 0;
    if (FAILED(InputData.Get_Data("CollisionType", iCollisionType)))
        return E_FAIL;

    if (FAILED(Bind_Collision(Cast<ECollisionType>(iCollisionType))))
        return E_FAIL;

    return S_OK;
}

HRESULT CColliderComponent::Initialize(ECollisionType eType)
{
    if (nullptr == m_pCollisionShape)
        return E_FAIL;

    // �浹ü�� ���� ����
    m_pCollisionShape->Set_Owner(this);

    // �浹ü�� �浹 �̺�Ʈ �߰��ϱ�
    m_pCollisionShape->Set_CollisionEvent(MakeDelegate(this, &ThisClass::OnCollision));

    return S_OK;
}

HRESULT CColliderComponent::Initialize(FSerialData& InputData)
{

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

    m_bIsCollision = false;
    //Update_Physics();
}

void CColliderComponent::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CColliderComponent::Render()
{
#ifdef _DEBUG
    if (nullptr == m_pCollisionShape)
        return E_FAIL;

    m_pBatch->Begin();

    m_pEffect->SetWorld(XMMatrixIdentity());
    m_pEffect->SetView(PipelineComp().Get_CamMatrix(ECamType::Persp, ECamMatrix::View, ECamNum::One));
    m_pEffect->SetProjection(PipelineComp().Get_CamMatrix(ECamType::Persp, ECamMatrix::Proj, ECamNum::One));

    D3D11Context()->IASetInputLayout(m_pInputLayout.Get());

    m_pEffect->Apply(D3D11Context());

    m_pCollisionShape->Render(m_pBatch, m_bIsCollision == true ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

    m_pBatch->End();
#endif


    return S_OK;
}

void CColliderComponent::EnterToPhysics(_uint iIndex)
{
    if (nullptr == m_pCollisionShape)
        return;

    m_iPhysics3dWorld_Index = iIndex;
    m_pGI->Add_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::ExitFromPhysics(_uint iIndex)
{
    if (nullptr == m_pCollisionShape)
        return;

    m_pGI->Delete_ColliderToPhysicsWorld(iIndex, m_pCollisionShape);
}

void CColliderComponent::Update_Physics()
{
    _matrix vMatrix = Calculate_TransformMatrixFromParent();
    _float4x4 vfMatrix = {};
    XMStoreFloat4x4(&vfMatrix, vMatrix);

    XMStoreFloat3x4(&m_pCollisionShape->matOffset, vMatrix);
}

void CColliderComponent::OnCollision(void* pDst, const FContact* pContact)
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
            m_bIsCollision = true;
            m_Collision_Event(pObj, pContact);
        }
    }
        
}

void CColliderComponent::OnCollisionEntered(void* pDst, const FContact* pContact)
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