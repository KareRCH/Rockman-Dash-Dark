#include "MainApp.h"

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
	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(g_hInst, g_hWnd), E_FAIL);

	DX11DEVICE_T tDevice = { m_pGI->Get_GraphicDev(), m_pGI->Get_GraphicContext() };

	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Default", L"πŸ≈¡", 15, 20, FW_HEAVY), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Jinji", L"±√º≠", 30, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"Font_Thin_Jinji", L"±√º≠", 18, 30, FW_THIN), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Font(L"MonsterUI", L"«‘√ ∑’πŸ≈¡", 14, 25, FW_THIN), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Initialize_TextureMgr(tDevice, L"Resource/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ModelMgr("Resource/Model/"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Initialize_ShaderMgr(tDevice, L"Shader/"), E_FAIL);

	FAILED_CHECK_RETURN(m_pGI->Create_Frame(L"Frame", 60.f), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_Immediate"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGI->Create_Timer(L"Timer_FPS"), E_FAIL);

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

#ifdef _DEBUG
	//Render_FrameRate();
#endif

	m_pGI->Present();
}

void CMainApp::Free()
{
	// dll ΩÃ±€≈Ê ¡¶∞≈
	Safe_Release(m_pGI);
}
