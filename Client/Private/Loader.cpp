#include "Loader.h"

#include "System/GameInstance.h"
#include "Component/NavigationComponent.h"

#include "process.h"

_uint APIENTRY LoadingThread(void* pArg)
{
	if (FAILED(CoInitializeEx(nullptr, 0)))
		return 0;

	CLoader* pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

CLoader::CLoader()
{

}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	/* �����带 �����Ѵ�. */
	/* LoadingThread : ������ �����尡 ���� ���� ȣ���ؾ��� �Լ� */
	/* this : �� �Լ��� ȣ���ϸ鼭 ���ڷ� ������ ������. */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingThread, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

CLoader* CLoader::Create(LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader();

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	DeleteObject(m_hThread);
	CloseHandle(m_hThread);
}

void CLoader::Print_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT hr = 0;

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Logo_Level();
		break;

	case LEVEL_GAMEPLAY:
		hr = Loading_For_GamePlay_Level();
		break;
	}

	if (FAILED(hr))
	{
		LeaveCriticalSection(&m_CriticalSection);
		return E_FAIL;
	}

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo_Level()
{
	// �ΰ�� ���ҽ� �ε�

	m_bIsFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay_Level()
{
	// �Ľ��� ���� �÷��� ���� �ε�


	// ���� ������Ʈ �ε�
	/* For.Prototype_Component_Navigation */
	if (FAILED(GI()->Add_PrototypeComp(TEXT("GamePlay"), TEXT("Prototype_Component_Navigation"),
		CNavigationComponent::Create(TEXT("Resource/Navigation.dat")))))
		return E_FAIL;

	m_bIsFinished = true;

	return S_OK;
}
