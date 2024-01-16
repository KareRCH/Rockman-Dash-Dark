#include "MainApp.h"

#include "Client_Define.h"
#include "Engine_Define.h"

#include "TestScene.h"
#include "System/GraphicDev.h"
#include "System/ShaderMgr_Enum.h"
#include "Level/Level_Loading.h"
#include "Component/TeamAgentComp.h"

#include "DirectXColors.h"

#include "Utility/RapidJsonSerial.h"

IMPLEMENT_SINGLETON(CMainApp)

CMainApp::CMainApp()
	: m_pGI(Engine::GameInstance())
{
	Safe_AddRef(m_pGI);
}

CMainApp* CMainApp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("MainApp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

HRESULT CMainApp::Initialize()
{
	// 장치 초기화에 쓰이는 구조체
	FDEVICE_INIT tDeviceInit = {};
	tDeviceInit.hWnd = g_hWnd;
	tDeviceInit.bVSync = false;
	tDeviceInit.bFullScreen = false;
	tDeviceInit.iScreenWidth = g_iWindowSizeX;
	tDeviceInit.iScreenHeight = g_iWindowSizeY;
	tDeviceInit.fScreenDepth = 1000.f;
	tDeviceInit.fScreenNear = 0.1f;
	tDeviceInit.iRenderTargetCount = 2;
	tDeviceInit.strMainPath = L"./";

	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(g_hInst, g_hWnd, tDeviceInit), E_FAIL);

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Add_Font(TEXT("Default"), TEXT("DungGeunMo-30.spritefont")), E_FAIL);

	GI()->Load_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	GI()->Load_Effect(L"Runtime/FX_Terrain.hlsl", SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);

	//GI()->Toggle_Deferred();

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 60.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

	CTeamAgentComp::Add_TeamRelation(ETEAM_PLAYER, ETEAM_ENEMY, ETeamRelation::Hostile);
	CTeamAgentComp::Add_TeamRelation(ETEAM_ENEMY, ETEAM_PLAYER, ETeamRelation::Hostile);

	FAILED_CHECK_RETURN(Open_Level(LEVEL_LOGO), E_FAIL);

	/*FSerialData Level;
	Level.Add_Member("Test", 1);
	Level.Add_Member("Test", 1.f);

	FSerialData Object;
	Object.Add_MemberString("Name", "Test");
	Object.Add_Member("Pos", 0.f);
	Level.Pushback_Member("Objects", Object);
	Level.Pushback_Member("Objects", Object);
	Level.Pushback_Member("Objects", Object);
	Level.Save_Data(TEXT("Resource/Test.json"));*/
	
	return S_OK;
}

void CMainApp::Priority_Tick(const _float& fTimeDelta)
{
	m_pGI->Tick_KeyMgr();
	m_pGI->StartFrame_PhysicsMgr();

	m_pGI->Priority_Tick_Object(fTimeDelta);
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGI->Tick_Object(fTimeDelta);

	m_pGI->Tick_PipelineMgr();
	m_pGI->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::Late_Tick(const _float& fTimeDelta)
{
	m_pGI->Late_Tick_Object(fTimeDelta);
	m_pGI->Tick_LevelMgr(fTimeDelta);
	m_pGI->Late_Tick_KeyMgr();
	m_pGI->Late_Tick_InputDev();
}

void CMainApp::Render()
{
	m_pGI->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.0f, 1.f));

	m_pGI->Clear_DepthStencil_View();

	//m_pGameInstance->Render_Scene(m_pDeviceContext);
	//m_pDeviceContext->Begin(nullptr);
	m_pGI->Render();
	//m_pDeviceContext->End(nullptr);

#ifdef _DEBUG
	Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll 싱글톤 제거
	m_pGI->Release_Managers();
	Safe_Release(m_pGI);
}

void CMainApp::Render_FrameRate()
{
	_float fFrameRate = m_pGI->Get_FrameRate(L"Frame");
	_float fTimeDelta = m_pGI->Get_TimerDelta(L"Timer_FPS");
	_float fResultFrameRate = Cast<_float>(fFrameRate / (fTimeDelta * fFrameRate));

	wstringstream ss;
	ss << (_uint)fResultFrameRate;
	wstring str = ss.str();
	_float2 vFontPos = { 0.f, 0.f };
	_vector vColorFont;

	if (fResultFrameRate >= fFrameRate * 0.9f)
		vColorFont = Colors::Green;
	else if (fResultFrameRate >= fFrameRate * 0.5f)
		vColorFont = Colors::Orange;
	else
		vColorFont = Colors::Red;

	m_pGI->Render_Font(TEXT("Default"), str.c_str(), _float2(0.f, 0.f), vColorFont);
}

HRESULT CMainApp::Open_Level(LEVEL eStartLevelID)
{
	if (nullptr == m_pGI)
		return E_FAIL;

	// 로딩 레벨에서 시작, 그 후 선택된 레벨로 로드한다.
	CLevel* pLevel = CLevel_Loading::Create(eStartLevelID);
	if (nullptr == pLevel)
		return E_FAIL;

	return m_pGI->Open_Level(LEVEL_LOADING, pLevel);
}
