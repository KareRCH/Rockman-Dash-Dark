#include "MainApp.h"

#include "Tool_Define.h"
#include "Engine_Define.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "System/Management.h"
#include "System/GraphicDev.h"

#include "ImGuiWin/ImGuiWin_Docking.h"
#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Hierarchi.h"
#include "ImGuiWin/ImGuiWin_Property.h"
#include "System/ImGuiMgr.h"
#include "Level/Level_MapTool.h"

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
	FAILED_CHECK_RETURN(Engine::GI()->Initialize(g_hInst, g_hWnd), E_FAIL);
	GI()->Add_SystemViewport({ 0.f, 0.f, g_iWindowSizeX, g_iWindowSizeY, 0.f, 1.f });

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Jinji", L"±Ã¼­", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Thin_Jinji", L"±Ã¼­", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"MonsterUI", L"ÇÔÃÊ·Õ¹ÙÅÁ", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_TextureMgr(tDevice, L"../Client/Resource/"), E_FAIL);
	m_pGI->Load_Texture(L"Model/Character/RockVolnutt/Body.png", true);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ModelMgr("../Client/Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ShaderMgr(tDevice, L"../Client/Shader/"), E_FAIL);
	m_pGI->Load_Shader(L"Compiled/PS_ModelTest.cso", EShaderType::Pixel, L"PS_ModelTest");
	m_pGI->Load_Shader(L"Compiled/VS_ModelTest.cso", EShaderType::Vertex, L"VS_ModelTest");
	m_pGI->Load_Effect(L"Runtime/FX_ModelTest.hlsl", L"FX_ModelTest", VERTEX_MODEL_SKIN_T::InputLayout, VERTEX_MODEL_SKIN_T::iMaxIndex);
	m_pGI->Load_Effect(L"Runtime/FX_Terrain.hlsl", L"FX_Terrain", VERTEX_NORM_T::InputLayout, VERTEX_NORM_T::iMaxIndex);

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 60.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_ImGuiMgr({ g_hWnd, tDevice.pDevice.Get(), tDevice.pDeviceContext.Get() }), E_FAIL);
	ImGui::SetCurrentContext(m_pGI->Get_ImGuiContext());
	m_pGI->Add_ImGuiWinAsRoot(TEXT("DockingSpace"), CImGuiWin_Docking::Create());
	m_pGI->Add_ImGuiWinAsChild(TEXT("DockingSpace"), TEXT("MapTool"), CImGuiWin_MapTool::Create());
	m_pGI->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("Viewer"), CImGuiWin_Viewer::Create());
	m_pGI->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Hierarchi"), CImGuiWin_Hierarchi::Create());
	m_pGI->Add_ImGuiWinAsChild(TEXT("MapTool"), TEXT("MapTool_Property"), CImGuiWin_Property::Create());

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

	m_pGI->Tick_ImGuiMgr(fTimeDelta);
	
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
	m_pGI->Render_ImGuiMgr();

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll ½Ì±ÛÅæ Á¦°Å
	Safe_Release(m_pGI);
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
