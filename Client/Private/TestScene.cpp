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

    GameInstance()->Load_Texture(L"Model/Character/RockVolnutt/Body.png", L"RockVolnutt", L"Body-BaseColor", true);
    GameInstance()->Load_Texture(L"Model/Character/RockVolnutt/Head.png", L"RockVolnutt", L"Head-BaseColor", true);
    GameInstance()->Load_Model("Character/RockVolnutt/Test.fbx", L"RockVolnutt");

    CLayer* pLayer = CLayer::Create(0.f);
    Add_Layer(L"GameLogic", pLayer);

    pLayer->Add_GameObject(CTestObject::Create({ m_pDevice, m_pDeviceContext }));
    pLayer->Add_GameObject(CDynamicCamera::Create({ m_pDevice, m_pDeviceContext }));

    return S_OK;
}

void CTestScene::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CTestScene::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    return 0;
}

void CTestScene::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
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
