#include "TestScene.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"

CTestScene::CTestScene(ID3D11Device* pGraphicDev)
    : Base(pGraphicDev)
{
}

HRESULT CTestScene::Initialize()
{
    FAILED_CHECK_RETURN(SUPER::Initialize(), E_FAIL);

    CLayer* pLayer = CLayer::Create(0.f);
    Add_Layer(L"GameLogic", pLayer);

    pLayer->Add_GameObject(CTestObject::Create(m_pDevice));
    pLayer->Add_GameObject(CDynamicCamera::Create(m_pDevice));

    return S_OK;
}

_int CTestScene::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    return 0;
}

void CTestScene::LateTick()
{
    SUPER::LateTick();
}

void CTestScene::Render(ID3D11DeviceContext* const pDeviceContext)
{
    SUPER::Render(pDeviceContext);

}

HRESULT CTestScene::InitializeLate_Scene()
{


    return S_OK;
}

CTestScene* CTestScene::Create(ID3D11Device* const pGraphicDev)
{
    ThisClass* pInstance = new ThisClass(pGraphicDev);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("TestScene Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CTestScene::Free()
{
    SUPER::Free();
}

HRESULT CTestScene::Initialize_Layer_Completed()
{
    return S_OK;
}
