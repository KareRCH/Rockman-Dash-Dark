#include "Level/Level_Loading.h"

#include "Loader.h"

HRESULT CLevel_Loading::Initialize()
{
	m_pLoader = CLoader::Create();
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(const _float& fTimeDelta)
{
	/*if (m_pLoader->IsFinished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			CLevel* pNewLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pNewLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pNewLevel)
				return;

			if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pNewLevel)))
				return;
		}
	}*/
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Print_LoadingText();

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create()
{
	CLevel_Loading* pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	Safe_Release(m_pLoader);
}
