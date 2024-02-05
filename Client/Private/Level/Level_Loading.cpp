#include "Level/Level_Loading.h"

#include "Loader.h"
#include "Level/Level_Logo.h"
#include "Level/Level_GamePlay.h"
#include "Level/Level_Parsed.h"

#include "GameObject/LoadingScreen.h"
#include "Component/PlaneModelComp.h"
#include "Component/RectBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CLevel_Loading::CLevel_Loading()
{
	Set_Name(TEXT("Loading"));
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	if (FAILED(Ready_Objects()))
		return E_FAIL;

	m_eNextLevelID = eNextLevelID;

	m_pLoader = CLoader::Create(eNextLevelID, TEXT(""));
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath)
{
	if (FAILED(Ready_Objects()))
		return E_FAIL;

	m_eNextLevelID = eNextLevelID;
	m_strParsedLevelDataPath = strParsedLevelDataPath;

	m_pLoader = CLoader::Create(eNextLevelID, strParsedLevelDataPath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	return S_OK;
}

void CLevel_Loading::Tick(const _float& fTimeDelta)
{
	if (nullptr == m_pGI)
		return;

	if (m_pLoader->IsFinished())
	{
		CLevel* pNewLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create();
			break;
		case LEVEL_GAMEPLAY:
			pNewLevel = CLevel_GamePlay::Create();
			break;
		case LEVEL_PARSED:
			pNewLevel = CLevel_Parsed::Create(m_strParsedLevelDataPath);
			break;
		}

		if (nullptr == pNewLevel)
			return;

		if (FAILED(m_pGI->Open_Level(m_eNextLevelID, pNewLevel)))
			return;
	}
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Print_LoadingText();

    return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CLevel_Loading* CLevel_Loading::Create(LEVEL eNextLevelID, const wstring& strParsedLevelDataPath)
{
	CLevel_Loading* pInstance = new CLevel_Loading();

	if (FAILED(pInstance->Initialize(eNextLevelID, strParsedLevelDataPath)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	SUPER::Free();

	Safe_Release(m_pLoader);
}

HRESULT CLevel_Loading::Ready_Objects()
{
	m_pGI->Set_LevelTag(Get_Name());

	CLoadingScreen* pLoadingScreen = { nullptr };

	if (FAILED(m_pGI->Add_GameObject(pLoadingScreen = CLoadingScreen::Create())))
		return E_FAIL;

	CPlaneModelComp* pModel = pLoadingScreen->Get_Component<CPlaneModelComp>(TEXT("LoadingScreen"));
	_float fX = 1024.f * (g_iWindowSizeX / 660.f);
	_float fY = 660.f * (g_iWindowSizeY / 512.f);
	pModel->Transform().Set_Scale(XMVectorSet(fX, fY, 1.f, 0.f));
	pModel->Transform().Set_Position(XMVectorSet(0.f, 0.f, 10.f, 1.f));

	return S_OK;
}
