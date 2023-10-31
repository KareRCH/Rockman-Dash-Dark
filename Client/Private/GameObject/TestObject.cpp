#include "GameObject/TestObject.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"

CTestObject::CTestObject(ID3D11Device* pGraphicDev)
    : Base(pGraphicDev)
{
}

CTestObject::CTestObject(const CGameObject& rhs)
    : Base(rhs)
{
}

HRESULT CTestObject::Initialize()
{
    FAILED_CHECK_RETURN(SUPER::Initialize(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

_int CTestObject::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    return 0;
}

void CTestObject::LateTick()
{
    SUPER::LateTick();
}

void CTestObject::Render(ID3D11DeviceContext* const pDeviceContext)
{
    SUPER::Render(pDeviceContext);

    m_ColorShaderComp->Render(pDeviceContext);
    m_TriBufferComp->Render(pDeviceContext);
}

CTestObject* CTestObject::Create(ID3D11Device* const pGraphicDev)
{
    ThisClass* pInstance = new ThisClass(pGraphicDev);

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
    Add_Component(L"Buffer", m_TriBufferComp = CTriBufferComp::Create(m_pDevice));
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    Add_Component(L"Shader", m_ColorShaderComp = CColorShaderComp::Create(m_pDevice));
    //m_ColorShaderComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);
    m_ColorShaderComp->Set_IndexCount(3);

    return S_OK;
}
