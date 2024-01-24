#include "GameObject/Door_Common.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CDoor_Common::CDoor_Common()
{
    Set_Name(TEXT("Door_Common"));
    Set_RenderGroup(ERenderGroup::NonBlend);
    Register_State();
}

CDoor_Common::CDoor_Common(const CDoor_Common& rhs)
{
    Register_State();
}

HRESULT CDoor_Common::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Map/Main/Door.amodel");

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
    m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
    m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
    m_pColliderComp->Bind_Collision(ECollisionType::OBB);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_OBJECT);

    return S_OK;
}

HRESULT CDoor_Common::Initialize_Prototype(FSerialData& InputData)
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
            //m_pColliderComp->EnterToPhysics(0);
            break;
        }
    }

    return S_OK;
}

HRESULT CDoor_Common::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CDoor_Common::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CDoor_Common::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CDoor_Common::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    m_State_Act.Get_StateFunc()(this, fTimeDelta);
    m_ActionKey.Reset();

    m_pColliderComp->Tick(fTimeDelta);
}

void CDoor_Common::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CDoor_Common::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

CDoor_Common* CDoor_Common::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CDoor_Common* CDoor_Common::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CDoor_Common::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CDoor_Common::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Door_Common Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CDoor_Common::Free()
{
    SUPER::Free();
}

FSerialData CDoor_Common::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

FSerialData CDoor_Common::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

HRESULT CDoor_Common::Initialize_Component()
{
    return S_OK;
}

HRESULT CDoor_Common::Initialize_Component(FSerialData& InputData)
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
            m_pColliderComp->EnterToPhysics(0);
            break;
        }
    }

    return S_OK;
}

void CDoor_Common::OnCollision(CGameObject* pDst, const FContact* const pContact)
{
}

void CDoor_Common::OnCollisionEntered(CGameObject* pDst, const FContact* const pContact)
{
}

void CDoor_Common::OnCollisionExited(CGameObject* pDst)
{
}

void CDoor_Common::OpenDoor()
{
    m_ActionKey.Act(EActionKey::Open);
}

void CDoor_Common::Register_State()
{
    for (_uint i = 0; i < ECast(EActionKey::Size); i++)
        m_ActionKey.Add_Action(Cast<EActionKey>(i));

    m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
    m_State_Act.Add_Func(EState_Act::Open, &ThisClass::ActState_Open);
    m_State_Act.Add_Func(EState_Act::Close, &ThisClass::ActState_Close);
    m_State_Act.Set_State(EState_Act::Idle);
}

void CDoor_Common::ActState_Idle(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(0, 1.f, false);
    }

    if (m_State_Act.Can_Update())
    {
        if (m_ActionKey.IsAct(EActionKey::Open))
            m_State_Act.Set_State(EState_Act::Open);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CDoor_Common::ActState_Open(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_fDoorClose.Reset();
        m_pModelComp->Set_Animation(1, 1.f, false);
        
    }

    if (m_State_Act.Can_Update())
    {
        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_pColliderComp->ExitFromPhysics(0);

        if (m_fDoorClose.Increase(fTimeDelta))
            m_State_Act.Set_State(EState_Act::Close);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}

void CDoor_Common::ActState_Close(const _float& fTimeDelta)
{
    if (m_State_Act.IsState_Entered())
    {
        m_pModelComp->Set_Animation(2, 1.f, false);
        m_pColliderComp->EnterToPhysics(0);
    }

    if (m_State_Act.Can_Update())
    {

        if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
            m_State_Act.Set_State(EState_Act::Idle);
    }

    if (m_State_Act.IsState_Exit())
    {

    }
}
