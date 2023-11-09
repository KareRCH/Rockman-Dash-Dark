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

    return S_OK;
}

void CTestObject::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    Set_Position(_float3(0.f, 0.f, 10.f));
    //Set_Scale(_float3(5.f, 5.f, 5.f));
    Calculate_Transform();

    GameInstance()->Add_RenderGroup(ERENDER_T::PERSPECTIVE, this);

    return 0;
}

void CTestObject::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

void CTestObject::Render()
{
    SUPER::Render();

    MATRIX_BUFFER_T matBuffer = { Get_Transform(),
        GameInstance()->Get_PerspectiveViewMatrix(0), GameInstance()->Get_PerspectiveProjMatrix(0) };
    CAMERA_BUFFER_T cameraBuffer = { _float3(10.f, 10.f, 10.f) };
    LIGHT_BUFFER_T lightBuffer = { _float4(0.2f, 0.2f, 0.2f, 1.f), _float4(0.2f, 0.2f, 0.2f, 1.f), _float3(-1.f, 0.f, 0.f),
                                    _float(2.f), _float4(1.f, 0.2f, 0.2f, 1.f)};

    m_pModelBufferComp->Render();
    m_pModelShaderComp->Render(matBuffer, cameraBuffer, lightBuffer);
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
    FAILED_CHECK_RETURN(Add_Component(L"Buffer", m_pModelBufferComp = CModelBufferComp::Create({ m_pDevice, m_pDeviceContext })), E_FAIL);
    m_pModelBufferComp->Initialize(L"RockVolnutt", L"Body");
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    FAILED_CHECK_RETURN(Add_Component(L"Shader", m_pModelShaderComp = CModelShaderComp::Create({ m_pDevice, m_pDeviceContext }, g_hWnd)), E_FAIL);
    //m_ColorShaderComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);
    m_pModelShaderComp->Set_IndexCount(m_pModelBufferComp->Get_IndexCount());
    ID3D11ShaderResourceView* pTest= GameInstance()->Get_Texture(L"RockVolnutt", L"Body-BaseColor");
    m_pModelShaderComp->Set_Texture(pTest);

    return S_OK;
}
