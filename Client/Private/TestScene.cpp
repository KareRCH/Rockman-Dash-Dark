#include "TestScene.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"
#include "System/Define/ModelMgr_Define.h"

HRESULT CTestScene::Initialize()
{
    FAILED_CHECK_RETURN(SUPER::Initialize(), E_FAIL);

    GameInstance()->Load_Texture(L"Model/Character/RockVolnutt/Body.png", L"RockVolnutt", L"Body-BaseColor", true);
    GameInstance()->Load_Texture(L"Model/Character/RockVolnutt/Head.png", L"RockVolnutt", L"Head-BaseColor", true);
    GameInstance()->Load_Model(EModelGroupIndex::Permanent, "Character/RockVolnutt/Test.fbx", L"RockVolnutt");

    CLayer* pLayer = CLayer::Create(0.f);
    Add_Layer(L"GameLogic", pLayer);

    pLayer->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 10.f)));
    pLayer->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 9.f)));
    pLayer->Add_GameObject(CDynamicCamera::Create());

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

HRESULT CTestScene::Render()
{
    SUPER::Render();

    return S_OK;
}

HRESULT CTestScene::InitializeLate_Scene()
{
    return S_OK;
}

CTestScene* CTestScene::Create()
{
    ThisClass* pInstance = new ThisClass();

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
