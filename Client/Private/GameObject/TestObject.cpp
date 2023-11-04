#include "GameObject/TestObject.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/ModelShaderComp.h"

#include "System/RenderMgr.h"

CTestObject::CTestObject(ID3D11Device* const pDevice)
    : Base(pDevice)
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

_int CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    Set_Position(_float3(0.f, 0.f, 10.f));
    //Set_Scale(_float3(5.f, 5.f, 5.f));
    Calculate_Transform();

    GameInstance()->Add_RenderGroup(ERENDER_T::PERSPECTIVE, this);

    return 0;
}

void CTestObject::LateTick()
{
    SUPER::LateTick();
}

void CTestObject::Render(ID3D11DeviceContext* const pDeviceContext)
{
    SUPER::Render(pDeviceContext);

    m_pModelBufferComp->Render(pDeviceContext);
    m_pModelShaderComp->Render(pDeviceContext, Get_Transform(),
        GameInstance()->Get_PerspectiveViewMatrix(0), GameInstance()->Get_PerspectiveProjMatrix(0));
}

CTestObject* CTestObject::Create(ID3D11Device* const pDevice)
{
    ThisClass* pInstance = new ThisClass(pDevice);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("TestObject Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CTestObject::Free()
{
    SUPER::Free();
}

HRESULT CTestObject::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Buffer", m_pModelBufferComp = CModelBufferComp::Create(m_pDevice)), E_FAIL);
    m_pModelBufferComp->Initialize("RockVolnut", "Legs");
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    FAILED_CHECK_RETURN(Add_Component(L"Shader", m_pModelShaderComp = CModelShaderComp::Create(m_pDevice, g_hWnd)), E_FAIL);
    //m_ColorShaderComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);
    m_pModelShaderComp->Set_IndexCount(m_pModelBufferComp->Get_IndexCount());

    return S_OK;
}
