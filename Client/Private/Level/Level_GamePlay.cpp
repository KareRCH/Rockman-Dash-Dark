#include "Level/Level_GamePlay.h"

#include "GameObject/TestObject.h"
#include "GameObject/DynamicCamera.h"
#include "BaseClass/Terrain.h"
#include "System/Define/ModelMgr_Define.h"
#include "System/GameInstance.h"
#include "Component/TerrainModelComp.h"
#include "GameObject/SkyBox.h"
#include "Component/CylinderBufferComp.h"

CLevel_GamePlay::CLevel_GamePlay()
{
    Set_Name(TEXT("Logo"));
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Objects()))
		return E_FAIL;

	return S_OK;
}

void CLevel_GamePlay::Tick(const _float& fTimeDelta)
{

}

HRESULT CLevel_GamePlay::Render()
{
	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	SUPER::Free();
}

HRESULT CLevel_GamePlay::Ready_Objects()
{
    m_pGI->Set_LevelTag(Get_Name());

    GI()->Load_Texture(L"Model/Character/Megaman/Image_%d.png", 2, true);
    GI()->Load_Model(EModelGroupIndex::Permanent, L"Model/Character/Megaman/Megaman.amodel");

    GI()->Add_GameObject(CTestObject::Create(_float3(10.f, 0.f, 10.f)));
    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 1.f)));
    GI()->Add_GameObject(CDynamicCamera::Create());

    CTerrain* pTerrain = { nullptr };
    GI()->Add_GameObject(pTerrain = CTerrain::Create());
    CTerrain::FInitTerrain tInit = {};
    tInit.strHeightMapPath = TEXT("TestHeight.png");
    tInit.iMaxWidth = 100;
    pTerrain->Create_TerrainByHeightMap(tInit);
    pTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"))->Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, TEXT("Textures/Study/Terrain/Grass_1.dds"), 1);

    CSkyBox* pSkyBox = { nullptr };
    GI()->Add_GameObject(pSkyBox = CSkyBox::Create());
    auto pModel = pSkyBox->Get_Component<CCylinderModelComp>(TEXT("ModelComp"));
    pModel->VIBufferComp()->Create_Buffer({ 10 });
    pModel->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/SkyBox/IceSea.dds"), 1);
    pModel->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxCube.hlsl"), SHADER_VTX_CUBETEX::Elements, SHADER_VTX_CUBETEX::iNumElements);

	return S_OK;
}
