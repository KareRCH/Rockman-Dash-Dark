#include "MainApp.h"

#include "Engine_Define.h"

IMPLEMENT_SINGLETON(CMainApp)

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
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
	FAILED_CHECK_RETURN(Engine::GameInstance()->Initialize(), E_FAIL);
	NULL_CHECK_RETURN(m_pGameInstance = Engine::GameInstance(), E_FAIL);

	FAILED_CHECK_RETURN(m_pGameInstance->Initialize_GraphicDev(g_iWindowSizeX, g_iWindowSizeY, false, g_hWnd, false, 1.f, 0.01f), E_FAIL);

	m_pGraphicDev = m_pGameInstance->Get_GraphicDev();
	Safe_AddRef(m_pGraphicDev);

	return S_OK;
}

_int CMainApp::Update(const _float& fTimeDelta)
{
	m_pGameInstance->Tick_InputDev();

	return 0;
}

void CMainApp::LateUpdate()
{

}

void CMainApp::Render()
{
	m_pGameInstance->Render_Begin(0.f, 0.f, 0.f, 1.f);

	m_pGameInstance->Render_End();
}

void CMainApp::Free()
{
	// 장치 제거
	Safe_Release(m_pGraphicDev);

	// dll 싱글톤 제거
	Safe_Release(m_pGameInstance);
}
