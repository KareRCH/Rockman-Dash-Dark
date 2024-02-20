#include "Loader.h"

#include "System/GameInstance.h"
#include "Component/NavigationComponent.h"
#include "Utility/RapidJsonSerial.h"
#include "GameObject/GameObjectFactory.h"
#include "GameObject/ComponentFactory.h"
#include "GameObject/LoadingScreen.h"

#include "process.h"

_uint APIENTRY LoadingThread(void* pArg)
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return 0;

	CLoader* pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

CLoader::CLoader()
{

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	/* 스레드를 생성한다. */
	/* LoadingThread : 생성한 스레드가 가장 먼저 호출해야할 함수 */
	/* this : 위 함수를 호출하면서 인자로 전달할 데이터. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	m_strParsedLevelDataPath = strParsedLevelDataPath;

	return S_OK;
}

CLoader* CLoader::Create(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath)
{
	CLoader* pInstance = new CLoader();

	if (FAILED(pInstance->Initialize(eNextLevelID, strParsedLevelDataPath)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);
}

void CLoader::Print_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT hr = 0;

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;

	case LEVEL_PARSED:
		hr = Loading_For_Parsed_Level();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(&m_CriticalSection);
		return E_FAIL;
	}

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	// 로고용 리소스 로드

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	// 파싱형 게임 플레이 레벨 로드


	// 원형 컴포넌트 로드
	/* For.Prototype_Component_Navigation */
	if (FAILED(GI()->Add_PrototypeComp(TEXT("GamePlay"), TEXT("Prototype_Component_Navigation"),
		CNavigationComponent::Create(TEXT("Resource/Navigation.dat")))))
		return E_FAIL;

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Parsed_Level()
{
	FSerialData Data;
	Data.Load_Data(m_strParsedLevelDataPath);

	string strName;
	Data.Get_Data("Name", strName);
	wstring strConvertName = ConvertToWstring(strName);

	GI()->Set_LevelTag(strConvertName);


	GI()->Add_PrototypeComp(strConvertName, TEXT("Prototype_Component_Navigation"),
		CNavigationComponent::Create(TEXT("Resource/Navigation.dat")));
		//return E_FAIL;

	FSerialData PrototypeData;
	Data.Get_Data("Prototypes", PrototypeData);

	_uint iNumComponents = PrototypeData.Get_ArraySize("Components");
	for (_uint i = 0; i < iNumComponents; i++)
	{
		FSerialData ComponentData;
		PrototypeData.Get_ObjectFromArray("Components", i, ComponentData);

		string strProtoName = "";
		if (FAILED(ComponentData.Get_Data("ProtoName", strProtoName)))
			return E_FAIL;

		
		GI()->Add_PrototypeComp(strConvertName, ConvertToWstring(strProtoName),
			CComponentFactory::Create(ComponentData));
	}

	_uint iNumObjects = PrototypeData.Get_ArraySize("Objects");
	for (_uint i = 0; i < iNumObjects; i++)
	{
		FSerialData ObjectData;
		PrototypeData.Get_ObjectFromArray("Objects", i, ObjectData);

		string strProtoName = "";
		if (FAILED(ObjectData.Get_Data("ProtoName", strProtoName)))
			return E_FAIL;

		GI()->Add_PrototypeObject(strConvertName, ConvertToWstring(strProtoName),
			CGameObjectFactory::Create(ObjectData));
	}
	
	m_bIsFinished = true;

	return S_OK;
}
