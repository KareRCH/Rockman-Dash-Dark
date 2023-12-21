#include "TestScene.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"
#include "BaseClass/Terrain.h"
#include "System/Define/ModelMgr_Define.h"
#include "System/GameInstance.h"
#include "Component/TerrainModelComp.h"
#include "GameObject/SkyBox.h"

HRESULT CTestScene::Initialize()
{
    GI()->Load_Texture(L"Model/Character/RockVolnutt/Body.png", true);
    GI()->Load_Texture(L"Model/Character/RockVolnutt/Head.png", true);
    GI()->Load_Model(EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Test.amodel");

    GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 0.f)));
    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 1.f)));
    GI()->Add_GameObject(CDynamicCamera::Create());

    CTerrain* pTerrain = { nullptr };
    GI()->Add_GameObject(pTerrain = CTerrain::Create());
    CTerrain::FInitTerrain tInit = {};
    tInit.strHeightMapPath = TEXT("TestHeight.png");
    tInit.iMaxWidth = 100;
    pTerrain->Create_TerrainByHeightMap(tInit);
    pTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"))->Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, TEXT("Textures/Study/Terrain/Grass_1.dds"));

    CSkyBox* pSkyBox = { nullptr };
    GI()->Add_GameObject(pSkyBox = CSkyBox::Create());
    auto pModel = pSkyBox->Get_Component<CBoxModelComp>(TEXT("ModelComp"));
    pModel->TextureComp()->Bind_Texture(TEXT("Textures/Study/SkBox/Sky_3.dds"));
    pModel->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxCube.hlsl"), SHADER_VTX_CUBETEX::InputLayout, SHADER_VTX_CUBETEX::iMaxIndex);

    return S_OK;
}

void CTestScene::Tick(const _float& fTimeDelta)
{

}

HRESULT CTestScene::Render()
{

    return S_OK;
}

CTestScene* CTestScene::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestScene Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

void CTestScene::Free()
{
}
