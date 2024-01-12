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
	// ����� �����ϴ� ������ ����.
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
		OutputDebugString(L" : Ŭ���� �����̼� �߰� ����!\n");
#endif // _DEBUG
		// ���н� �����带 ���� ��Ų��.
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
		OutputDebugString(L" : Ŭ���� �����̼��� ���� ���� ����!\n");
#endif // _DEBUG
		return E_FAIL;
	}

	// ���� ��Ű�� �����ϱ�, �ٸ� ������ ���� �˾Ƽ� ��ũ�� ���� �ȴ�.
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
		OutputDebugString(L" : Ŭ���� �����̼� ã�� ����!\n");
#endif // _DEBUG
		return nullptr;
	}

	return (*iter).second;
}