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


	DX11DEVICE_T tDevice;
	tDevice.pDevice = m_pGameInstance->Get_GraphicDev();
	tDevice.pDeviceContext = m_pGameInstance->Get_GraphicContext();

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_InputDev(g_hInst, g_hWnd), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_RenderMgr(tDevice), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_PhysicsMgr(1), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_SoundMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FontMgr(tDevice), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Default", L"����", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Jinji", L"�ü�", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"Font_Thin_Jinji", L"�ü�", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(L"MonsterUI", L"���ʷչ���", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TextureMgr(tDevice, L"../Client/Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ModelMgr("../Client/Resource/Model/"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_KeyMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FrameMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Frame(L"Frame", 120.f), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TimerMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_FPS"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_CloudStationMgr(), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ComponentMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ObjectMgr(), E_FAIL);

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
	// ��ġ ����
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	// dll �̱��� ����
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
