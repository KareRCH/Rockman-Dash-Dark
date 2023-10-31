#include "MainApp.h"

#include "Client_Define.h"
#include "Engine_Define.h"

#include "System/Management.h"

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

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_GraphicDev(g_iWindowSizeX, g_iWindowSizeY, false, g_hWnd, false, 1.f, 0.01f), E_FAIL);

	m_pDevice = m_pGameInstance->Get_GraphicDev();
	Safe_AddRef(m_pDevice);

	m_pDeviceContext = m_pGameInstance->Get_GraphicContext();
	Safe_AddRef(m_pDeviceContext);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_InputDev(g_hInst, g_hWnd), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_KeyMgr(), E_FAIL);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_PhysicsMgr(1), E_FAIL);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_SoundMgr(), E_FAIL);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FontMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(m_pDevice, L"Font_Default", L"¹ÙÅÁ", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(m_pDevice, L"Font_Jinji", L"±Ã¼­", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(m_pDevice, L"Font_Thin_Jinji", L"±Ã¼­", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Font(m_pDevice, L"MonsterUI", L"ÇÔÃÊ·Õ¹ÙÅÁ", 14, 25, FW_THIN), E_FAIL);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_FrameMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Frame(L"Frame", 60.f), E_FAIL);


	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TimerMgr(), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Create_Timer(L"Timer_FPS"), E_FAIL);

	

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_Management(EMANAGE_SCENE::SINGLE), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_BlackBoardMgr(), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_TextureMgr(m_pDevice), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_ProtoMgr(), E_FAIL);

	return S_OK;
}

_int CMainApp::Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Tick_KeyMgr();

	m_pGameInstance->StartFrame_PhysicsMgr();

	//m_pGameInstance->Tick_Scene(fTimeDelta);

	m_pGameInstance->Tick_PhysicsMgr(fTimeDelta);

	return 0;
}

void CMainApp::LateTick()
{
	//m_pGameInstance->LateTick_Scene();
	m_pGameInstance->LateTick_KeyMgr();
}

void CMainApp::Render()
{
	m_pGameInstance->Render_Begin(0.f, 0.f, 0.f, 1.f);

	//m_pGameInstance->Render_Scene(m_pGraphicDev);

#ifdef _DEBUG
	Render_FrameRate();
#endif

	m_pGameInstance->Render_End();
}

void CMainApp::Free()
{
	// ÀåÄ¡ Á¦°Å
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	// dll ½Ì±ÛÅæ Á¦°Å
	Safe_Release(m_pGameInstance); 
	Release_GameInstance();
}

void CMainApp::Render_FrameRate()
{
	_float fFrameRate = m_pGameInstance->Get_FrameRate(L"Frame");
	_float fTimeDelta = m_pGameInstance->Get_TimerDelta(L"Timer_FPS");
	_float fResultFrameRate = Cast<_float>(fFrameRate / (fTimeDelta * fFrameRate));

	wstringstream ss;
	ss << fResultFrameRate;
	wstring str = ss.str();
	_vec2 vFontPos = { 0.f, 0.f };
	D3DCOLOR colorFont;

	if (fResultFrameRate >= fFrameRate * 0.9f)
		colorFont = DXCOLOR_GREEN;
	else if (fResultFrameRate >= fFrameRate * 0.5f)
		colorFont = DXCOLOR_ORANGE;
	else
		colorFont = DXCOLOR_RED;

	m_pGameInstance->Render_Font(L"Font_Jinji", str.c_str(), &vFontPos, colorFont);
}
