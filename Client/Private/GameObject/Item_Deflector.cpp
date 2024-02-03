#include "GameObject/Item_Deflector.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/StaticObject.h"
#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"

CItem_Deflector::CItem_Deflector()
{
    Set_Name(TEXT("Item_Deflector"));
}

CItem_Deflector::CItem_Deflector(const CItem_Deflector& rhs)
{
}

HRESULT CItem_Deflector::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.7f, 0.7f, 0.7f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Deflector/Deflector.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
    m_pColliderComp->Transform().Set_Scale(0.35f, 0.7f, 0.35f);
    m_pColliderComp->Transform().Set_PositionY(0.7f);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_ITEM);
    m_pColliderComp->Set_CollisionMask(COLLAYER_WALL | COLLAYER_FLOOR);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData ProtoData;
        InputData.Get_ObjectFromArray("Components", i, ProtoData);

        _uint iComponentID = 0;
        if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strName = "";
        if (FAILED(ProtoData.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::CommonModel):
            NULL_CHECK_RETURN(m_pModelComp = CCommonModelComp::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pModelComp)))
                return E_FAIL;
            m_pModelComp->Set_Animation(0, 1.f, true);
            break;
        case ECast(EComponentID::Collider):
            NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pColliderComp)))
                return E_FAIL;
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            break;
        }
    }

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

    return S_OK;
}

HRESULT CItem_Deflector::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CItem_Deflector::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CItem_Deflector::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_ActionKey.Reset();


    m_State_Act.Get_StateFunc()(this, fTimeDelta);

    Update_Move(fTimeDelta);

    m_pColliderComp->Tick(fTimeDelta);
}

void CItem_Deflector::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CItem_Deflector::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

void CItem_Deflector::BeginPlay()
{
    SUPER::BeginPlay();

    m_RandomNumber = mt19937_64(m_RandomDevice());

    m_vAcceleration = m_vMoveSpeed = m_vMaxMoveSpeed = { 6.f, 10.f, 6.f };
    m_vAcceleration = { 100.f, g_fGravity, 100.f };

    Set_RenderGroup(ERenderGroup::Blend);
    Register_State();
    m_pModelComp->Set_Animation(0, 1.f, true);
    if (m_pColliderComp)
        m_pColliderComp->EnterToPhysics(0);

    switch (m_eType)
    {
    case EType::White:
        m_iGiveMoney = 10;
        m_pModelComp->Transform().Set_Scale(_float3(0.2f, 0.2f, 0.2f));
        break;
    case EType::Yellow:
        m_iGiveMoney = 100;
        m_pModelComp->Transform().Set_Scale(_float3(0.4f, 0.4f, 0.4f));
        break;
    case EType::Green:
        m_iGiveMoney = 500;
        m_pModelComp->Transform().Set_Scale(_float3(0.6f, 0.6f, 0.6f));
        break;
    case EType::Blue:
        m_iGiveMoney = 1000;
        m_pModelComp->Transform().Set_Scale(_float3(0.8f, 0.8f, 0.8f));
        break;
    case EType::Purple:
        m_iGiveMoney = 5000;
        m_pModelComp->Transform().Set_Scale(_float3(1.f, 1.f, 1.f));
        break;
    }

    uniform_real_distribution<_float> RandomVeloX(-1.5f, 1.5f);
    uniform_real_distribution<_float> RandomVeloZ(-1.5f, 1.5f);

    m_vVelocity.y = 3.f;
    m_vVelocity.x = RandomVeloX(m_RandomNumber);
    m_vVelocity.z = RandomVeloZ(m_RandomNumber);
}

CItem_Deflector* CItem_Deflector::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CItem_Deflector* CItem_Deflector::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Deflector::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CItem_Deflector::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CItem_Deflector::Free()
{
    SUPER::Free();
}

HRESULT CItem_Deflector::Initialize_Component()
{
    

    return S_OK;
}

HRESULT CItem_Deflector::Initialize_Component(FSerialData& InputData)
{
    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData InputProto;
        InputData.Get_ObjectFromArray("Components", i, InputProto);

        _uint iComponentID = 0;
        if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strProtoName = "";
        if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
            return E_FAIL;

        string strName = "";
        if (FAILED(InputProto.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::CommonModel):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pModelComp = DynCast<CCommonModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pModelComp->Set_Animation(0, 1.f, true);
            break;
        case ECast(EComponentID::Collider):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pColliderComp = DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
            m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
            m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
            break;
        }
    }

    return S_OK;
}

FSerialData CItem_Deflector::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

FSerialData CItem_Deflector::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

void CItem_Deflector::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    CStaticObject* pSolid = DynCast<CStaticObject*>(pDst);
    if (nullptr != pSolid)
    {
        _float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
        _vector vSimNormal = {};
        vSimNormal = XMLoadFloat3(&vNormal);
        Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
        if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
            m_bIsOnGround = true;
    }
}

void CItem_Deflector::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
}

void CItem_Deflector::OnCollisionExited(CGameObject* pDst)
{
}

void CItem_Deflector::Register_State()
{
    for (_uint i = 0; i < ECast(EActionKey::Size); i++)
        m_ActionKey.Add_Action(Cast<EActionKey>(i));

    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Ready_Floor, &ThisClass::ActState_Ready_Floor);
    m_State_Act.Add_Func(EState_Act::Flooring, &ThisClass::ActState_Flooring);
    m_State_Act.Set_State(EState_Act::Idle);
}

void CItem_Deflector::Update_Move(const _float& fTimeDelta)
{
    _float3 vfPos = Transform().Get_PositionFloat3();
    _vector vPos = Transform().Get_PositionVector();

    m_vVelocity.y -= m_vAcceleration.y * fTimeDelta;
    vfPos.y += m_vVelocity.y * fTimeDelta;
    if (vfPos.y <= 0.f)
    {
        vfPos.y = 0.f;
        m_bIsOnGround = true;
    }
    else
        m_bIsOnGround = false;

    if (m_bIsOnGround)
    {
        m_vVelocity.x *= 0.5f;
        m_vVelocity.z *= 0.5f;
    }
    else
    {
        m_vVelocity.x *= 0.998f;
        m_vVelocity.z *= 0.998f;
    }

    Transform().Set_Position(vPos + XMLoadFloat3(&m_vVelocity) * fTimeDelta);
    Transform().Set_PositionY(vfPos.y);
}

void CItem_Deflector::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, true);
    }

    if (m_State_Act.Can_Update())
    {
        // 돌진
        if (m_bIsOnGround)
            m_State_Act.Set_State(EState_Act::Ready_Floor);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CItem_Deflector::ActState_Ready_Floor(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(1, 1.f, false, false, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        // 돌진
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Flooring);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CItem_Deflector::ActState_Flooring(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(2, 1.f, true, false, 1.f);
    }

    if (m_State_Act.Can_Update())
    {
        Transform().TurnRight(10.f * fTimeDelta);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CItem_Deflector::Obtain_Money(_uint& iMoney)
{
    iMoney += m_iGiveMoney;
}
