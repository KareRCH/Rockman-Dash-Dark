#include "System/FrameMgr.h"

CFrameMgr::CFrameMgr()
{
}

HRESULT CFrameMgr::Initialize()
{
	return S_OK;
}

CFrameMgr* CFrameMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("FrameMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CFrameMgr::Free()
{
	for (auto iter = m_mapFrame.begin(); iter != m_mapFrame.end(); ++iter)
	{
		Safe_Release((*iter).second);
	}
	m_mapFrame.clear();
}

_bool CFrameMgr::IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame* pFrame = Find_Frame(pFrameTag);
	NULL_CHECK_RETURN(pFrame, false);

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT CFrameMgr::Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame* pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pFrame, E_FAIL);

	m_mapFrame.insert({ pFrameTag, pFrame });

	return S_OK;
}

CFrame* CFrameMgr::Find_Frame(const _tchar* pFrameTag)
{
	auto	iter = m_mapFrame.find(pFrameTag);

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void CFrameMgr::Set_FrameRate(const _tchar* pFrameTag, const _float& fFrameRate)
{
	CFrame* pFrame = Find_Frame(pFrameTag);
	if (pFrame == nullptr)
		return;

	pFrame->Set_FrameRate(fFrameRate);
}

const _float CFrameMgr::Get_FrameRate(const _tchar* pFrameTag)
{
	CFrame* pFrame = Find_Frame(pFrameTag);
	if (pFrame == nullptr)
		return _float(0.f);

	return pFrame->Get_FrameRate();
}


