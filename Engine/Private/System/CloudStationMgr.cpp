#include "System/CloudStationMgr.h"

CCloudStationMgr::CCloudStationMgr()
{
}

HRESULT CCloudStationMgr::Initialize()
{
	return S_OK;
}

void CCloudStationMgr::Tick()
{
	// 만료시 삭제하는 블랙보드 삭제.
	for (auto iter = m_mapCloudStations.begin(); iter != m_mapCloudStations.end();)
	{
		if ((*iter).second->IsExpired())
		{
			Safe_Release((*iter).second);
			iter = m_mapCloudStations.erase(iter);
		}
		else
			++iter;
	}
}

CCloudStationMgr* CCloudStationMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("BlackBoardMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CCloudStationMgr::Free()
{
	for (auto itemPair : m_mapCloudStations)
		Safe_Release(itemPair.second);
	m_mapCloudStations.clear();
}

HRESULT CCloudStationMgr::Add_CloudStation(const wstring& strBoardName, CCloudStation* pBlackBoard)
{
	auto iter = m_mapCloudStations.emplace(strBoardName, pBlackBoard);
	if (!iter.second)
	{
#ifdef _DEBUG
		OutputDebugString(strBoardName.c_str());
		OutputDebugString(L" : 클라우드 스테이션 추가 못함!\n");
#endif // _DEBUG
		// 실패시 블랙보드를 해제 시킨다.
		Safe_Release(pBlackBoard);
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CCloudStationMgr::Delete_CloudStation(const wstring& strBoardName)
{
	auto iter = m_mapCloudStations.find(strBoardName);
	if (iter == m_mapCloudStations.end())
	{
#ifdef _DEBUG
		OutputDebugString(strBoardName.c_str());
		OutputDebugString(L" : 클라우드 스테이션이 없어 삭제 실패!\n");
#endif // _DEBUG
		return E_FAIL;
	}

	// 만료 시키고 삭제하기, 다른 곳에서 이제 알아서 링크를 끊게 된다.
	(*iter).second->Set_Expired();
	m_mapCloudStations.erase(iter);

	return S_OK;
}

CCloudStation* CCloudStationMgr::Get_CloudStation(const wstring& strBoardName)
{
	auto iter = m_mapCloudStations.find(strBoardName);
	if (iter == m_mapCloudStations.end())
	{
#ifdef _DEBUG
		OutputDebugString(strBoardName.c_str());
		OutputDebugString(L" : 클라우드 스테이션 찾지 못함!\n");
#endif // _DEBUG
		return nullptr;
	}

	return (*iter).second;
}