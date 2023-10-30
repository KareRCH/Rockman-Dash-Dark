#include "System/TimerMgr.h"

CTimerMgr::CTimerMgr()
{
}

HRESULT CTimerMgr::Initialize()
{
	return S_OK;
}

CTimerMgr* CTimerMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("PhysicsMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CTimerMgr::Free()
{
	for (auto iter = m_mapTimers.begin(); iter != m_mapTimers.end(); ++iter)
	{
		Safe_Release((*iter).second);
	}
	m_mapTimers.clear();
}

_float CTimerMgr::Get_TimeDelta(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	NULL_CHECK_RETURN(pTimer, 0.f);

	return pTimer->Get_TimeDelta();
}

void CTimerMgr::Set_TimeDelta(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	NULL_CHECK(pTimer);

	pTimer->SetUp_TimeDelta();
}

HRESULT CTimerMgr::Create_Timer(const _tchar* pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	NULL_CHECK_RETURN(pTimer, E_FAIL);

	m_mapTimers.insert({ pTimerTag, pTimer });

	return S_OK;
}

CTimer* CTimerMgr::Find_Timer(const _tchar* pTimerTag) const
{
	auto	iter = m_mapTimers.find(pTimerTag);

	if (iter == m_mapTimers.end())
		return nullptr;

	return iter->second;
}


