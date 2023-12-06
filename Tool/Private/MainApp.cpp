#include "MainApp.h"

#include "Tool_Define.h"
#include "Engine_Define.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "System/Management.h"
#include "System/GraphicDev.h"

#include "ImGuiWin/ImGuiWin_Docking.h"
#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "System/ImGuiMgr.h"

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
	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(g_hInst, g_hWnd), E_FAIL);

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Default", L"����", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Jinji", L"�ü�", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Thin_Jinji", L"�ü�", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"MonsterUI", L"���ʷչ���", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_TextureMgr(tDevice, L"Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ModelMgr("Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ShaderMgr(tDevice, L"Shader/"), E_FAIL);
	GameInstance()->Load_Shader(L"Compiled/PS_ModelTest.cso", EShaderType::Pixel, L"PS_ModelTest");
	GameInstance()->Load_Shader(L"Compiled/VS_ModelTest.cso", EShaderType::Vertex, L"VS_ModelTest");
	GI()->Load_Effect(L"Runtime/FX_ModelTest.hlsl", L"FX_ModelTest", VERTEX_MODEL_SKIN_T::InputLayout, VERTEX_MODEL_SKIN_T::iMaxIndex);
	GI()->Load_Effect(L"Runtime/FX_Terrain.hlsl", L"FX_Terrain", VERTEX_NORM_T::InputLayout, VERTEX_NORM_T::iMaxIndex);

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 60.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_ImGuiMgr({ g_hWnd, tDevice.pDevice.Get(), tDevice.pDeviceContext.Get() }), E_FAIL);
	ImGui::SetCurrentContext(m_pGI->Get_ImGuiContext());
	m_pGI->Add_ImGuiWin(TEXT("DockingSpace"), CImGuiWin_Docking::Create(), true);
	m_pGI->Add_ImGuiWin(TEXT("MapTool"), CImGuiWin_MapTool::Create(), true);
	m_pGI->AttachToChild_ImGuiWin(TEXT("DockingSpace"), TEXT("MapTool"));
	
	return S_OK;
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGI->Tick_KeyMgr();
	
	m_pGI->StartFrame_PhysicsMgr();

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
	m_pGI->Render();
	m_pGI->Render_ImGuiMgr();

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll �̱��� ����
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
