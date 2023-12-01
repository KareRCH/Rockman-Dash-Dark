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
    return S_OK;
}

HRESULT CTestObject::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용
    TurnOn_State(EGObjectState::RenderDeferred);    // 디퍼드 셰이딩 사용, ZBuffer 미사용시 무시

    return S_OK;
}

HRESULT CTestObject::Initialize(const _float3 vPos)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    Transform().Set_Position(vPos);

    return S_OK;
}

void CTestObject::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_W))
        Transform().Set_PositionZ(Transform().Get_PositionFloat3().z + 5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_S))
        Transform().Set_PositionZ(Transform().Get_PositionFloat3().z - 5.f * fTimeDelta);

    if (GI()->IsKey_Pressed(DIK_RETURN))
        Toggle_State(EGObjectState::Render);
    if (GI()->IsKey_Pressed(DIK_BACK))
        Set_Dead();
    
    Transform().Set_Scale(_float3(1.f, 1.f, 1.f));
    
    _float3 t = Transform().Get_RotationEulerFloat3();

    return 0;
}

void CTestObject::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

void CTestObject::Render()
{
    SUPER::Render();

    m_pModelComp->Render();
}

CTestObject* CTestObject::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CTestObject* CTestObject::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(vPos)))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);

        return nullptr;
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

        return nullptr;
    }

    /* Arg에서 넘겨받은 데이터 적용하기 */

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

    m_pModelComp->Bind_Model(EModelGroupIndex::Permanent, L"RockVolnutt");
    m_pModelComp->Bind_Mesh(L"Body");
    m_pModelComp->Bind_Mesh(L"Arms");
    m_pModelComp->Bind_Mesh(L"Legs");
    m_pModelComp->Bind_Mesh(L"Head");
    m_pModelComp->Bind_Effect(L"FX_ModelTest");
    m_pModelComp->Bind_Skeletal(L"Armature");

    //ID3D11ShaderReflectionVariable* pVariable = 
    //m_pModelComp->m_pShaderComp->
    //ID3D11ShaderReflection*
    //FAILED_CHECK_RETURN(Add_Component(L"Shader", m_pModelShaderComp = CModelShaderComp::Create(g_hWnd)), E_FAIL);
    ////m_ColorShaderComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);
    //m_pModelShaderComp->Set_IndexCount(m_pModelComp->VIBufferComp()->Get_IndexCount());
    //ID3D11ShaderResourceView* pTest = GameInstance()->Get_Texture(L"RockVolnutt", L"Body-BaseColor");
    //m_pModelShaderComp->Set_Texture(pTest);

    return S_OK;
}
