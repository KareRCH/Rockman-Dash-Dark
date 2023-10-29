#include "MainApp.h"

#include "Export_Engine.h"

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
	FAILED_CHECK_RETURN(Engine::Initialize_GraphicDev(g_iWindowSizeX, g_iWindowSizeY, false, g_hWnd, false, 1.f, 0.01f), E_FAIL);
	m_pDeviceClass = Engine::CGraphicDev::GetInstance();
	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_Device();
	m_pGraphicDev->AddRef();

	return S_OK;
}

_int CMainApp::Update(const _float& fTimeDelta)
{
	return _int();
}

void CMainApp::LateUpdate()
{
}

void CMainApp::Render()
{
	Engine::Render_Begin(0.f, 0.f, 0.f, 1.f);

	Engine::Render_End();
}

void CMainApp::Free()
{
	// ¿Âƒ° ¡¶∞≈
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);

	// dll ΩÃ±€≈Ê ¡¶∞≈
	Engine::Release_Engine();
}
