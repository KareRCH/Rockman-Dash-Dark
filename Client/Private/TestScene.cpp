#include "TestScene.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"
#include "BaseClass/Terrain.h"
#include "System/Define/ModelMgr_Define.h"
#include "System/GameInstance.h"

HRESULT CTestScene::Initialize()
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    GI()->Load_Texture(L"Model/Character/RockVolnutt/Body.png", true);
    GI()->Load_Texture(L"Model/Character/RockVolnutt/Head.png", true);
    GI()->Load_Model(EModelGroupIndex::Permanent, "Character/RockVolnutt/Test.fbx", L"RockVolnutt");

    GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 0.f)));
    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 1.f)));
    GI()->Add_GameObject(CDynamicCamera::Create());
    GI()->Add_GameObject(CTerrain::Create());

    return S_OK;
}

_int CTestScene::Tick(const _float& fTimeDelta)
{
    return 0;
}

HRESULT CTestScene::Render()
{
    SUPER::Render();

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
