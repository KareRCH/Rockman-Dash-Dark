#include "MainApp.h"

#include "Tool_Define.h"
#include "Engine_Define.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "System/GraphicDev.h"

#include "ImGuiWin/ImGuiWin_Docking.h"
#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Hierarchi.h"
#include "ImGuiWin/ImGuiWin_Property.h"
#include "ImGuiWin/ImGuiWin_Browser.h"
#include "ImGuiWin/ImGuiWin_ObjectTool.h"
#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "ImGuiWin/ImGuiMgr.h"
#include "Level/Level_MapTool.h"

//#include "GameObject/DynamicCamera.h"

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
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

HRESULT CMainApp::Initialize()
{
	// ÀåÄ¡ ÃÊ±âÈ­¿¡ ¾²ÀÌ´Â ±¸Á¶Ã¼
	FDEVICE_INIT tDeviceInit = {};
	tDeviceInit.hWnd = g_hWnd;
	tDeviceInit.bVSync = false;
	tDeviceInit.bFullScreen = false;
	tDeviceInit.iScreenWidth = g_iWindowSizeX;
	tDeviceInit.iScreenHeight = g_iWindowSizeY;
	tDeviceInit.fScreenDepth = 1000.f;
	tDeviceInit.fScreenNear = 0.1f;
	tDeviceInit.iRenderTargetCount = 2;

	FAILED_CHECK_RETURN(Engine::GI()->Initialize(g_hInst, g_hWnd, tDeviceInit), E_FAIL);
	GI()->Add_SystemViewport({ 0.f, 0.f, g_iWindowSizeX, g_iWindowSizeY, 0.f, 1.f });
	// À§Ä¡, ¿ÀºêÁ§Æ® ID¸¦ ¹ÝÈ¯ÇÏ±â À§ÇØ ¾²ÀÌ´Â ·»´õÅ¸°Ù
	D3D11_TEXTURE2D_DESC Desc = *GI()->Get_RTV_Desc(1);
	Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GI()->Set_RTV_Desc(1, Desc);

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Jinji", L"±Ã¼­", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Thin_Jinji", L"±Ã¼­", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"MonsterUI", L"ÇÔÃÊ·Õ¹ÙÅÁ", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_TextureMgr(tDevice, L"../Client/Resource/"), E_FAIL);
	m_pGI->Load_Texture(L"Model/Character/RockVolnutt/Body.png", true);
	m_pGI->Load_Texture(L"Model/Character/RockVolnutt/Head.png", true);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ModelMgr("../Client/Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ShaderMgr(tDevice, L"../Client/Shader/"), E_FAIL);
	m_pGI->Load_Shader(L"Compiled/PS_ModelTest.cso", EShaderType::Pixel, L"PS_ModelTest");
	m_pGI->Load_Shader(L"Compiled/VS_ModelTest.cso", EShaderType::Vertex, L"VS_ModelTest");
	m_pGI->Load_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::InputLayout, SHADER_VTX_SKINMODEL::iMaxIndex);

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 120.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(CImGuiMgr::GetInstance()->Initialize({ g_hWnd, tDevice.pDevice.Get(), tDevice.pDeviceContext.Get() }), E_FAIL);
	ImGui::SetCurrentContext(CImGuiMgr::GetInstance()->Get_GuiContext());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsRoot(TEXT("DockingSpace"), CImGuiWin_Docking::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("DockingSpace"), TEXT("MapTool"), CImGuiWin_MapTool::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Viewer"), CImGuiWin_Viewer::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Hierarchi"), CImGuiWin_Hierarchi::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Property"), CImGuiWin_Property::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Browser"), CImGuiWin_Browser::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Terrain"), CImGuiWin_Terrain::Create());
	CImGuiMgr::GetInstance()->Add_ImGuiWinAsChild(TEXT("DockingSpace"), TEXT("ObjectTool"), CImGuiWin_ObjectTool::Create());

	m_pGI->Open_Level(0, CLevel_MapTool::Create());

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

	CImGuiMgr::GetInstance()->Tick(fTimeDelta);

	m_pGI->Tick_PipelineMgr();
	
	m_pGI->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::Late_Tick(const _float& fTimeDelta)
{
	m_pGI->Late_Tick_Object(fTimeDelta);
	m_pGI->Late_Tick_KeyMgr();
}

void CMainApp::Render()
{
	m_pGI->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.0f, 1.f));
	m_pGI->Clear_DepthStencil_View();
	m_pGI->Bind_SystemViewport(0);
	m_pGI->Render();
	CImGuiMgr::GetInstance()->Render();

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll ½Ì±ÛÅæ Á¦°Å
	Safe_Release(m_pGI);
	CImGuiMgr::GetInstance()->DestroyInstance();
}

void CMainApp::Render_FrameRate()
{
	_float fFrameRate = m_pGI->Get_FrameRate(L"Frame");
	_float fTimeDelta = m_pGI->Get_TimerDelta(L"Timer_FPS");
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

	m_pGI->Render_Font(L"Font_Jinji", str.c_str(), &vFontPos, colorFont);
}
