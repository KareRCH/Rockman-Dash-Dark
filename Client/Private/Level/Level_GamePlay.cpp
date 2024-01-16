#include "Level/Level_GamePlay.h"

#include "GameObject/Player.h"
#include "GameObject/DynamicCamera.h"
#include "BaseClass/Terrain.h"
#include "System/Define/ModelMgr_Define.h"
#include "System/GameInstance.h"
#include "Component/TerrainModelComp.h"
#include "GameObject/SkyBox.h"
#include "Component/CylinderBufferComp.h"
#include "GameObject/ItemChest.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "BaseClass/Navigation.h"
#include "Component/NavigationComponent.h"
#include "GameObject/ReaverBot_Balfura.h"

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

	m_pGI->Play_BGM(TEXT("RockmanDash2"), TEXT("07. Calinca - Tundra.mp3"), 1.f);

	HANDLE		hFile = CreateFile(TEXT("Resource/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_ulong		dwByte = { 0 };

	_float3		vPoints[3] = {};

	vPoints[0] = _float3(0.f, 0.0f, 100.f);
	vPoints[1] = _float3(100.f, 0.0f, 0.0f);
	vPoints[2] = _float3(0.0f, 0.0f, 0.0f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.0f, 100.f);
	vPoints[1] = _float3(100.f, 0.0f, 100.f);
	vPoints[2] = _float3(100.f, 0.0f, 0.0f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.f, 0.0f, 200.f);
	vPoints[1] = _float3(100.f, 0.0f, 100.f);
	vPoints[2] = _float3(0.0f, 0.0f, 100.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(100.f, 0.0f, 100.f);
	vPoints[1] = _float3(200.f, 0.0f, 0.f);
	vPoints[2] = _float3(100.f, 0.0f, 0.0f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);



    //GI()->Add_GameObject(CTestObject::Create(_float3(0.f, 0.f, 1.f)));
    GI()->Add_GameObject(CDynamicCamera::Create());

    CTerrain* pTerrain = { nullptr };
    GI()->Add_GameObject(pTerrain = CTerrain::Create());
    CTerrain::FInitTerrain tInit = {};
    tInit.strHeightMapPath = TEXT("TestHeight.png");
    tInit.iMaxWidth = 100;
    pTerrain->Create_TerrainByHeightMap(tInit);
    pTerrain->Get_Component<CTerrainModelComp>(TEXT("TerrainModelComp"))->Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, TEXT("Textures/RockmanDash2/Terrain.png"), 1);

    CSkyBox* pSkyBox = { nullptr };
    GI()->Add_GameObject(pSkyBox = CSkyBox::Create());
    auto pModel = pSkyBox->Get_Component<CCylinderModelComp>(TEXT("ModelComp"));
    pModel->VIBufferComp()->Create_Buffer({ 10 });
    pModel->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/SkyBox/IceSea.dds"), 1);
    pModel->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxCube.hlsl"), SHADER_VTX_CUBETEX::Elements, SHADER_VTX_CUBETEX::iNumElements);


	CNavigationComponent::TCloneDesc tDesc = { 0 };
	GI()->Add_GameObject(CNavigation::Create());

	return S_OK;
}
