#include "TestScene.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"

CTestScene::CTestScene(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
}

HRESULT CTestScene::Initialize()
{
    FAILED_CHECK_RETURN(SUPER::Initialize(), E_FAIL);

    GameInstance()->Load_Model("Character/RockVolnut/Test.fbx", "RockVolnut");

    CLayer* pLayer = CLayer::Create(0.f);
    Add_Layer(L"GameLogic", pLayer);

    pLayer->Add_GameObject(CTestObject::Create({ m_pDevice, m_pDeviceContext }));
    pLayer->Add_GameObject(CDynamicCamera::Create({ m_pDevice, m_pDeviceContext }));

    

    return S_OK;
}

void CTestScene::PriorityTick()
{
    SUPER::PriorityTick();
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

void CTestScene::Render()
{
    SUPER::Render();
}

HRESULT CTestScene::InitializeLate_Scene()
{


    return S_OK;
}

CTestScene* CTestScene::Create(const DX11DEVICE_T tDevice)
{
    ThisClass* pInstance = new ThisClass(tDevice);

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
