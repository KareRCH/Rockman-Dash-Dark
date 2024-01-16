#include "Component/CloudStationComp.h"

#include "System/CloudStationMgr.h"

CCloudStationComp::CCloudStationComp(const CCloudStationComp& rhs)
	: Base(rhs)
{
}

HRESULT CCloudStationComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

    return S_OK;
}

HRESULT CCloudStationComp::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	return S_OK;
}

HRESULT CCloudStationComp::Initialize(void* Arg)
{
    return S_OK;
}

HRESULT CCloudStationComp::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;

	return S_OK;
}

void CCloudStationComp::Priority_Tick(const _float& fTimeDelta)
{
	if (nullptr != m_pCloudStationMgr)
	{
		// 재연결 대기 시간
		if (m_fReconnectTime.Increase(fTimeDelta))
		{
			// 예약된 이름으로 클라우드 스테이션 찾기
			for (auto iter = m_ReservedStations.begin(); iter != m_ReservedStations.end();)
			{
				Connect_CloudStation((*iter).first);

				if ((*iter).second.Increase(fTimeDelta))
					iter = m_ReservedStations.erase(iter);
				else
					iter++;
			}
		}
	}
}

void CCloudStationComp::Tick(const _float& fTimeDelta)
{

}

void CCloudStationComp::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CCloudStationComp::Render()
{
	return S_OK;
}

FSerialData CCloudStationComp::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	Data.Add_Member("ComponentID", g_ClassID);

	return Data;
}

FSerialData CCloudStationComp::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ComponentID", g_ClassID);

	return Data;
}

CCloudStationComp* CCloudStationComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CloudStationComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CCloudStationComp* CCloudStationComp::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CloudStationComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCloudStationComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CloudStationComp Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

CComponent* CCloudStationComp::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CloudStationComp Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CCloudStationComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pCloudStationMgr);

	for (auto& CloudStation : m_CloudStations)
		Safe_Release(CloudStation);
	m_CloudStations.clear();
}

HRESULT CCloudStationComp::Open_CloudStation_Session(const wstring& strName, CCloudStation* pCloudStation)
{
	if (m_pGI == nullptr)
		return E_FAIL;

	m_pCloudStationMgr = m_pGI->Get_CloudStationMgr();
	if (m_pCloudStationMgr == nullptr)
		return E_FAIL;

	m_pCloudStationMgr->Add_CloudStation(strName, pCloudStation);

	return S_OK;
}

HRESULT CCloudStationComp::Connect_CloudStation(const wstring& strName)
{
	if (m_pGI == nullptr)
		return E_FAIL;

	m_pCloudStationMgr = m_pGI->Get_CloudStationMgr();
	if (m_pCloudStationMgr == nullptr)
		return E_FAIL;

	// 못 찾으면 실패
	CCloudStation* pCloudStation = m_pCloudStationMgr->Find_CloudStation(strName);
	if (nullptr == pCloudStation)
	{
		m_ReservedStations.emplace(strName, FGauge(10.f));
		return E_FAIL;
	}

	// 이미 연결된게 있으면 성공
	if (Exists_CloudStation(pCloudStation))
		return S_OK;

	// 추가
	m_CloudStations.push_back(pCloudStation);
	Safe_AddRef(pCloudStation);
	++m_iNumCloudStations;

#ifdef _DEBUG
	cout << ConvertToString(strName) << "에 연결 완료" << endl;
#endif

	return S_OK;
}

void CCloudStationComp::Disconnect_CloudStation(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumCloudStations)
		return;

	auto iter = m_CloudStations.begin() + iIndex;
	Safe_Release((*iter));
	m_CloudStations.erase(iter);
	--m_iNumCloudStations;
}

_bool CCloudStationComp::Exists_CloudStation(CCloudStation* pCloudStation)
{
	auto iterFind = find_if(m_CloudStations.begin(), m_CloudStations.end(),
		[&pCloudStation](CCloudStation* pDst) {
			return pCloudStation == pDst;
		}
	);
	if (iterFind == m_CloudStations.end())
		return false;

	return true;
}
