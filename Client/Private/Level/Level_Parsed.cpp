#include "Level/Level_Parsed.h"

#include "Utility/RapidJsonSerial.h"

#include "GameObject/GameObjectFactory.h"

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

CLevel_Parsed::CLevel_Parsed()
{
	Set_Name(TEXT("Parsed"));
}

CLevel_Parsed::CLevel_Parsed(const CLevel_Parsed& rhs)
{
}

HRESULT CLevel_Parsed::Initialize(const wstring& strParsedLevelDataPath)
{
	FSerialData Data;
	Data.Load_Data(strParsedLevelDataPath);

	string strName;
	Data.Get_Data("Name", strName);
	Set_Name(ConvertToWstring(strName));

	if (FAILED(Ready_Objects(Data)))
		return E_FAIL;

	return S_OK;
}

void CLevel_Parsed::Tick(const _float& fTimeDelta)
{
}

HRESULT CLevel_Parsed::Render()
{
    return S_OK;
}

CLevel_Parsed* CLevel_Parsed::Create(const wstring& strParsedLevelDataPath)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(strParsedLevelDataPath)))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Parsed::Free()
{
	SUPER::Free();

}

HRESULT CLevel_Parsed::Ready_Objects(FSerialData& Data)
{
	m_pGI->Set_LevelTag(Get_Name());

	_uint iNumObjects = Data.Get_ArraySize("Objects");
	for (_uint i = 0; i < iNumObjects; i++)
	{
		FSerialData ObjectData;
		Data.Get_ObjectFromArray("Objects", i, ObjectData);

		_uint iClassID;
		Data.Get_Data("ClassID", iClassID);
		m_pGI->Add_GameObject(CGameObjectFactory::Create(ObjectData));
		//Create_Object(iClassID, ObjectData);

		/*StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		ObjectData.m_Doc.Accept(writer);

		cout << buffer.GetString() << endl;*/
	}

	GI()->Add_GameObject(CItemChest::Create(_float3(15.f, 0.f, 10.f)));
	GI()->Add_GameObject(CReaverBot_Horokko::Create(_float3(10.f, 0.f, 15.f)));
	GI()->Add_GameObject(CReaverBot_Balfura::Create(_float3(11.f, 1.f, 16.f)));
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
