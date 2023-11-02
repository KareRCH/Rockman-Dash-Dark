#include "System/JobMgr.h"

CJobMgr::CJobMgr()
    : m_iThreadCount(thread::hardware_concurrency())
{
}

HRESULT CJobMgr::Initialize(const _uint iJobCount, const _uint iAsyncCount)
{
    m_iJobCount = iJobCount;
    m_iAsyncCount = iAsyncCount;

    return S_OK;
}

CJobMgr* CJobMgr::Create(const _uint iJobCount, const _uint iAsyncCount)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(iJobCount, iAsyncCount)))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("JobMgr Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CJobMgr::Free()
{
}
