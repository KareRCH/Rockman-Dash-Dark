#include "MainApp.h"

#include "ModelBinary_Define.h"
#include "Engine_Define.h"

#include "System/GraphicDev.h"
#include "System/ShaderMgr_Enum.h"
#include "ImGuiWin/ImGuiMgr.h"
#include "ImGuiWin/ImGuiWin_Convert.h"

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

		return nullptr;
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

	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(g_hInst, g_hWnd, tDeviceInit), E_FAIL);

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Initialize_FontMgr(tDevice, TEXT("../Client/Resource/Font/")), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_TextureMgr(tDevice, L"Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ModelMgr(L"Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ShaderMgr(tDevice, L"Shader/"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 60.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(CImGuiMgr::GetInstance()->Initialize({ g_hWnd, tDevice.pDevice.Get(), tDevice.pDeviceContext.Get() }), E_FAIL);
	ImGui::SetCurrentContext(CImGuiMgr::GetInstance()->Get_GuiContext());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsRoot(TEXT("Convert"), CImGuiWin_Convert::Create());
	/*CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("DockingSpace"), TEXT("MapTool"), CImGuiWin_MapTool::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Viewer"), CImGuiWin_Viewer::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Hierarchi"), CImGuiWin_Hierarchi::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Property"), CImGuiWin_Property::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Browser"), CImGuiWin_Browser::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Terrain"), CImGuiWin_Terrain::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("DockingSpace"), TEXT("ObjectTool"), CImGuiWin_ObjectTool::Create());*/

	return S_OK;
}

void CMainApp::Priority_Tick(const _float& fTimeDelta)
{
	m_pGI->Tick_KeyMgr();
	m_pGI->StartFrame_PhysicsMgr();

	CImGuiMgr::GetInstance()->Tick(fTimeDelta);

	m_pGI->Priority_Tick_Object(fTimeDelta);
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGI->Tick_Object(fTimeDelta);

	m_pGI->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::Late_Tick(const _float& fTimeDelta)
{
	m_pGI->Late_Tick_Object(fTimeDelta);
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

	CImGuiMgr::GetInstance()->Render();

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll 싱글톤 제거
	Safe_Release(m_pGI);
	CImGuiMgr::GetInstance()->DestroyInstance();
}
