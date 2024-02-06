#include "System/LevelMgr.h"

#include "BaseClass/Level.h"
#include "System/GameInstance.h"

HRESULT CLevelMgr::Initialize()
{
	return S_OK;
}

void CLevelMgr::Tick(const _float& fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_bIsLevelTransitioned = false;

	m_pCurrentLevel->Tick(fTimeDelta);
}

HRESULT CLevelMgr::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render();
}

CLevelMgr* CLevelMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("RenderMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevelMgr::Free()
{
	Safe_Release(m_pCurrentLevel);
}

HRESULT CLevelMgr::Open_Level(_uint iCurrentLevelIndex, CLevel* pNewLevel, _bool bIsNotLoading)
{
	if (nullptr == pNewLevel)
		return E_FAIL;

	// 씬 옮겨갈 때 오브젝트 없애기
	if (nullptr != m_pCurrentLevel)
		GI()->Clear(m_pCurrentLevel->Get_Name());

	if (0 != Safe_Release(m_pCurrentLevel))
		return E_FAIL;

	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iCurrentLevelIndex;

	m_bIsLevelTransitioned = true;
	m_bIsNotLoading = bIsNotLoading;

	return S_OK;
}
