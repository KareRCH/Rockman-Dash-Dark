#include "MainApp.h"

#include "Client_Define.h"
#include "Engine_Define.h"

#include "System/Management.h"
#include "TestScene.h"
#include "System/GraphicDev.h"
#include "System/ShaderMgr_Enum.h"

IMPLEMENT_SINGLETON(CMainApp)

CMainApp::CMainApp()
	: m_pGameInstance(Engine::GameInstance())
{
	Safe_AddRef(m_pGameInstance);
}

CMainApp* CMainApp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("MainApp Create Failed");

		return nullptr;
	}

	return pInstance;
}

HRESULT CMainApp::Initialize()
{
	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(), E_FAIL);

	FDEVICE_INIT tDeviceInit;
	tDeviceInit.hWnd = g_hWnd;
	tDeviceInit.bVSync = false;
	tDeviceInit.bFullScreen = false;
	tDeviceInit.iScreenWidth = g_iWindowSizeX;
	tDeviceInit.iScreenHeight = g_iWindowSizeY;
	tDeviceInit.fScreenDepth = 1000.f;
	tDeviceInit.fScreenNear = 0.1f;
	

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_GraphicDev(tDeviceInit), E_FAIL);

	m_pDevice = m_pGameInstance->Get_GraphicDev();
	Safe_AddRef(m_pDevice);

	m_pDeviceContext = m_pGameInstance->Get_GraphicContext();
	Safe_AddRef(m_pDeviceContext);

	DX11DEVICE_T tDevice = { m_pDevice, m_pDeviceContext };


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_InputDev(g_hInst, g_hWnd), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_RenderMgr(tDevice), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_PhysicsMgr(1), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_SoundMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FontMgr(tDevice), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Jinji", L"±Ã¼­", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Thin_Jinji", L"±Ã¼­", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"MonsterUI", L"ÇÔÃÊ·Õ¹ÙÅÁ", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TextureMgr(tDevice, L"Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ModelMgr("Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ShaderMgr(tDevice, L"Shader/"), E_FAIL);
	GameInstance()->Load_Shader(L"PS_ModelTest.cso", EShaderType::Pixel, L"PS_ModelTest");
	GameInstance()->Load_Shader(L"VS_ModelTest.cso", EShaderType::Vertex, L"VS_ModelTest");
	

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_KeyMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FrameMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Frame(L"Frame", 60.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TimerMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_BlackBoardMgr(), E_FAIL);

	

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ProtoMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ObjectMgr(tDevice, EMANAGE_SCENE::SINGLE), E_FAIL);
	m_pGameInstance->Set_Scene(CTestScene::Create(tDevice));
	

	return S_OK;
}

void CMainApp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Tick_KeyMgr();

	m_pGameInstance->StartFrame_PhysicsMgr();

	m_pGameInstance->Tick_Scene(fTimeDelta);

	m_pGameInstance->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Late_Tick_Scene(fTimeDelta);
	m_pGameInstance->Late_Tick_KeyMgr();
}

void CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.0f, 1.f));

	m_pGameInstance->Clear_DepthStencil_View();

	//m_pGameInstance->Render_Scene(m_pDeviceContext);
	//m_pDeviceContext->Begin(nullptr);
	m_pGameInstance->Render();
	//m_pDeviceContext->End(nullptr);

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGameInstance->Present();
}

void CMainApp::Free()
{
	// ÀåÄ¡ Á¦°Å
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	// dll ½Ì±ÛÅæ Á¦°Å
	Safe_Release(m_pGameInstance);
}

void CMainApp::Render_FrameRate()
{
	_float fFrameRate = m_pGameInstance->Get_FrameRate(L"Frame");
	_float fTimeDelta = m_pGameInstance->Get_TimerDelta(L"Timer_FPS");
	_float fResultFrameRate = Cast<_float>(fFrameRate / (fTimeDelta * fFrameRate));

	wstringstream ss;
	ss << fResultFrameRate;
	wstring str = ss.str();
	_float2 vFontPos = { 0.f, 0.f };
	D3DCOLOR colorFont;

	if (fResultFrameRate >= fFrameRate * 0.9f)
		colorFont = DXCOLOR_GREEN;
	else if (fResultFrameRate >= fFrameRate * 0.5f)
		colorFont = DXCOLOR_ORANGE;
	else
		colorFont = DXCOLOR_RED;

	m_pGameInstance->Render_Font(L"Font_Jinji", str.c_str(), &vFontPos, colorFont);
}
