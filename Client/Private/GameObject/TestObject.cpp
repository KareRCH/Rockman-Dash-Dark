#include "GameObject/TestObject.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"

#include "System/RenderMgr.h"

CTestObject::CTestObject()
{
    Set_Name(L"TestObject");
}

CTestObject::CTestObject(const CTestObject& rhs)
    : Base(rhs)
    , m_pModelComp(rhs.m_pModelComp)
{
}

HRESULT CTestObject::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용

    return S_OK;
}

HRESULT CTestObject::Initialize_Prototype(const _float3 vPos)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용

    Transform().Set_Position(vPos);

    return S_OK;
}

HRESULT CTestObject::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CTestObject::Initialize(const _float3 vPos)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    Transform().Set_Position(vPos);
    //Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));

    return S_OK;
}

void CTestObject::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

void CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_UP))
        Transform().MoveForward(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_DOWN))
        Transform().MoveForward(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_RIGHT))
        Transform().TurnRight(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_LEFT))
        Transform().TurnRight(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressed(DIK_RETURN))
        Toggle_State(EGObjectState::Render);
    if (GI()->IsKey_Pressed(DIK_BACK))
        Set_Dead();
    
    Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    
    _float3 t = Transform().Get_RotationEulerFloat3();

    if (m_Gauge.Increase(fTimeDelta))
    {
        
        m_Gauge.Reset();
    }

    if (GI()->IsKey_Pressed(DIK_T))
    {
        m_bTest = !m_bTest;
        if (m_bTest)
            m_pModelComp->Set_MaskAnimation(0, L"Megaman|anim_000_Megaman");
        else
            m_pModelComp->Set_MaskAnimation(0, L"Megaman|anim_001_Megaman");
    }
    
    m_pModelComp->Set_MaskTime(0, m_Gauge.fCur);
    m_pModelComp->Apply_Pose();
    m_pModelComp->Invalidate_BoneTransforms();
}

void CTestObject::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

HRESULT CTestObject::Render()
{
    SUPER::Render();
    
    m_pModelComp->Render();

    return S_OK;
}

CTestObject* CTestObject::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CTestObject* CTestObject::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTestObject::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CTestObject::Free()
{
    SUPER::Free();

    Safe_Release(m_pModelComp);
}

HRESULT CTestObject::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CSkinnedModelComp::Create()), E_FAIL);
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::InputLayout, SHADER_VTX_SKINMODEL::iMaxIndex);

    m_pModelComp->Bind_Model(EModelGroupIndex::Permanent, L"Model/Character/Megaman/Megaman.amodel");

    m_pModelComp->Bind_Mesh(L"Mesh_0", 0);
    m_pModelComp->Bind_Mesh(L"Mesh_0", 1);
    
    m_pModelComp->Bind_Skeletal(L"Armature");

    m_pModelComp->Create_Mask(L"Main", L"Armature", true);
    m_pModelComp->Create_Mask(L"Head", L"Armature", false);
    m_pModelComp->Create_Mask(L"Leg", L"Armature", false);
    m_pModelComp->Create_Mask(L"LeftArm", L"Armature", false);
    m_pModelComp->Create_Mask(L"RightArm", L"Armature", false);

    m_pModelComp->Active_BoneMask(2, L"bone_000");


    return S_OK;
}
