#include "System/ImGuiMgr.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "BaseClass/ImGuiWin.h"

CImGuiMgr::CImGuiMgr()
{
}

HRESULT CImGuiMgr::Initialize()
{
    return S_OK;
}

void CImGuiMgr::Tick(const _float fTimeDelta)
{
	for (_uint i = 0; i < m_vecRootWins.size(); i++)
	{
		m_vecRootWins[i]->Tick(fTimeDelta);
	}
}

HRESULT CImGuiMgr::Render()
{
	return S_OK;
}

CImGuiMgr* CImGuiMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiMgr::Free()
{
	for (auto& Pair : m_mapImGuiWin)
		Safe_Release(Pair.second);
	m_mapImGuiWin.clear();
	m_vecRootWins.clear();
}

HRESULT CImGuiMgr::ResetResolution()
{
	return S_OK;
}

HRESULT CImGuiMgr::Add_ImGuiWin(const wstring& strKey, CImGuiWin* pImGuiWin)
{


	return S_OK;
}
