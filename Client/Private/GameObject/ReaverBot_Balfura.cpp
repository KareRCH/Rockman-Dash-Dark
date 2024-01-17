#include "GameObject/ReaverBot_Balfura.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CReaverBot_Balfura::CReaverBot_Balfura()
{
    Set_Name(TEXT("ReaverBot_Balfura"));
    Set_RenderGroup(ERenderGroup::NonBlend);
}

CReaverBot_Balfura::CReaverBot_Balfura(const CReaverBot_Balfura& rhs)
    : Base(rhs)
{
    /*NULL_CHECK(m_pColliderComp = DynCast<CColliderComponent*>(rhs.m_pColliderComp->Clone()));
    NULL_CHECK(m_pModelComp = DynCast<CCommonModelComp*>(rhs.m_pModelComp->Clone()));*/
}

HRESULT CReaverBot_Balfura::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.32f, 0.32f, 0.32f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Balfura/Balfura.amodel");
    m_pModelComp->Set_Animation(0, 1.f, true);

    FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
    m_pColliderComp->Bind_Collision(ECollisionType::OBB);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
        | COLLAYER_ATTACKER | COLLAYER_OBJECT);

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize_Prototype(FSerialData& InputData)
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
            m_pColliderComp->EnterToPhysics(0);
            break;
        }
    }

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;
    

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

    return S_OK;
}

void CReaverBot_Balfura::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Balfura::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    //Input_ActionKey();
    ////m_State_AI.Get_StateFunc()(this, fTimeDelta);
    //m_State_Act.Get_StateFunc()(this, fTimeDelta);
    //m_ActionKey.Reset();

    m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Balfura::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Balfura::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

FSerialData CReaverBot_Balfura::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

FSerialData CReaverBot_Balfura::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);
    Data.Add_Member("HP", m_fHP.fMax);

    return Data;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CReaverBot_Balfura::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CReaverBot_Balfura::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Reaverbot_Balfura Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Balfura::Free()
{
    SUPER::Free();
}

HRESULT CReaverBot_Balfura::Initialize_Component()
{
    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize_Component(FSerialData& InputData)
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

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

void CReaverBot_Balfura::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

}

void CReaverBot_Balfura::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CReaverBot_Balfura::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}
