#include "GameObject/TestObject.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/ModelShaderComp.h"

#include "System/RenderMgr.h"

CTestObject::CTestObject(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
    Set_Name(L"TestObject");
}

CTestObject::CTestObject(const CGameObject& rhs)
    : Base(rhs)
{
}

HRESULT CTestObject::Initialize()
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

    Set_Position(vPos);

    return S_OK;
}

void CTestObject::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (GameInstance()->IsKey_Pressing(DIK_W))
        Set_Position(_float3(Get_Position().x, Get_Position().y, Get_Position().z + 5.f * fTimeDelta));
    else if (GameInstance()->IsKey_Pressing(DIK_S))
        Set_Position(_float3(Get_Position().x, Get_Position().y, Get_Position().z - 5.f * fTimeDelta));
    
    Set_Scale(_float3(2.f, 2.f, 2.f));
    Set_Rotation(_float3(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(180.f)));
    
    _float3 t = Get_Rotation();
    //m_pModelBufferComp->Calculate_TransformFromParent();

    return 0;
}

void CTestObject::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

void CTestObject::Render()
{
    SUPER::Render();

    MATRIX_BUFFER_T matBuffer = { XMLoadFloat4x4(m_pModelBufferComp->Get_Transform()) * XMLoadFloat4x4(Get_Transform()),
        GameInstance()->Get_PerspectiveViewMatrix(0), GameInstance()->Get_PerspectiveProjMatrix(0) };
    CAMERA_BUFFER_T cameraBuffer = { _float3(6.f, 6.f, 6.f) };
    LIGHT_BUFFER_T lightBuffer = { _float4(0.2f, 0.2f, 0.2f, 1.f), _float4(0.2f, 0.2f, 0.2f, 1.f), _float3(-1.f, 0.f, 0.f),
                                    _float(2.f), _float4(1.f, 0.2f, 0.2f, 1.f)};
    BONE_COMMON_BUFFER_T boneBuffer = {};

    m_pModelBufferComp->Render();
    m_pModelShaderComp->Render(matBuffer, cameraBuffer, boneBuffer, lightBuffer);
}

CTestObject* CTestObject::Create(const DX11DEVICE_T tDevice)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("TestObject Create Failed");

        return nullptr;
    }

    return pInstance;
}

CTestObject* CTestObject::Create(const DX11DEVICE_T tDevice, const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize(vPos)))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("TestObject Create Failed");

        return nullptr;
    }

    return pInstance;
}

CGameObject* CTestObject::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("TestObject Create Failed");

        return nullptr;
    }

    /* Arg에서 넘겨받은 데이터 적용하기 */

    return Cast<CGameObject*>(pInstance);
}

void CTestObject::Free()
{
    SUPER::Free();
}

HRESULT CTestObject::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Buffer", m_pModelBufferComp = CModelBufferComp::Create({ m_pDevice.Get(), m_pDeviceContext.Get()})), E_FAIL);
    m_pModelBufferComp->Initialize(EModelGroupIndex::Permanent, L"RockVolnutt", L"Body");
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    FAILED_CHECK_RETURN(Add_Component(L"Shader", m_pModelShaderComp = CModelShaderComp::Create({ m_pDevice.Get(), m_pDeviceContext.Get()}, g_hWnd)), E_FAIL);
    //m_ColorShaderComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);
    m_pModelShaderComp->Set_IndexCount(m_pModelBufferComp->Get_IndexCount());
    ID3D11ShaderResourceView* pTest= GameInstance()->Get_Texture(L"RockVolnutt", L"Body-BaseColor");
    m_pModelShaderComp->Set_Texture(pTest);

    return S_OK;
}
