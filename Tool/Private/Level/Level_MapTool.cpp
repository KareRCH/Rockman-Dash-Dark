#include "Level/Level_MapTool.h"

#include "BaseClass/Terrain.h"
#include "GameObject/ToolCamera.h"
#include "Component/NavigationComponent.h"

HRESULT CLevel_MapTool::Initialize()
{
    /*GI()->Load_Texture(L"Model/Character/RockVolnutt/Body.png", true);
    GI()->Load_Texture(L"Model/Character/RockVolnutt/Head.png", true);
    GI()->Load_Model(EModelGroupIndex::Permanent, "Character/RockVolnutt/Test.fbx", L"RockVolnutt");*/

    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 0.f)));
    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 1.f)));
    //GI()->Add_GameObject(CToolCamera::Create());
    //GI()->Add_GameObject(CTerrain::Create());
    GI()->Set_LevelTag(TEXT("MapTool"));

    if (FAILED(GI()->Add_PrototypeComp(TEXT("MapTool"), TEXT("Prototype_Component_Navigation"),
        CNavigationComponent::Create(TEXT("../Client/Resource/Navigation.dat")))))
        return E_FAIL;

    return S_OK;
}

void CLevel_MapTool::Tick(const _float& fTimeDelta)
{

}

HRESULT CLevel_MapTool::Render()
{
    return S_OK;
}

CLevel_MapTool* CLevel_MapTool::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestScene Create Failed");
        Engine::Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_MapTool::Free()
{
}
