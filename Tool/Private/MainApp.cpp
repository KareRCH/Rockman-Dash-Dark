#include "MainApp.h"

#include "Tool_Define.h"
#include "Engine_Define.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "System/Management.h"
#include "System/GraphicDev.h"

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
	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(g_hInst, g_hWnd), E_FAIL);

	DX11DEVICE_T tDevice = { m_pGameInstance->Get_GraphicDev(), m_pGameInstance->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Jinji", L"±Ã¼­", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Thin_Jinji", L"±Ã¼­", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"MonsterUI", L"ÇÔÃÊ·Õ¹ÙÅÁ", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TextureMgr(tDevice, L"Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ModelMgr("Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ShaderMgr(tDevice, L"Shader/"), E_FAIL);
	GameInstance()->Load_Shader(L"Compiled/PS_ModelTest.cso", EShaderType::Pixel, L"PS_ModelTest");
	GameInstance()->Load_Shader(L"Compiled/VS_ModelTest.cso", EShaderType::Vertex, L"VS_ModelTest");
	GI()->Load_Effect(L"Runtime/FX_ModelTest.hlsl", L"FX_ModelTest", VERTEX_MODEL_SKIN_T::InputLayout, VERTEX_MODEL_SKIN_T::iMaxIndex);
	GI()->Load_Effect(L"Runtime/FX_Terrain.hlsl", L"FX_Terrain", VERTEX_NORM_T::InputLayout, VERTEX_NORM_T::iMaxIndex);

	FAILED_CHECK_RETURN(m_pGameInstance->Create_Frame(L"Frame", 60.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_FPS"), E_FAIL);

	//FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ImGuiMgr(), E_FAIL);

	return S_OK;
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Tick_KeyMgr();

	m_pGameInstance->StartFrame_PhysicsMgr();

	m_pGameInstance->Tick_Object(fTimeDelta);

	m_pGameInstance->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Late_Tick_Object(fTimeDelta);
	m_pGameInstance->Late_Tick_KeyMgr();
}

void CMainApp::Render()
{
	m_pGameInstance->Clear_BackBuffer_View(_float4(0.0f, 0.f, 1.0f, 1.f));

	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Render();

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
