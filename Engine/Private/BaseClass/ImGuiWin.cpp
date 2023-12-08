#include "BaseClass/ImGuiWin.h"

HRESULT CImGuiWin::Initialize()
{
    return S_OK;
}

void CImGuiWin::Tick(const _float& fTimeDelta)
{
	for (_uint i = 0; i < m_vecChildrenWin.size(); i++)
	{
		m_vecChildrenWin[i]->Tick(fTimeDelta);
	}
}

HRESULT CImGuiWin::Render()
{
	for (_uint i = 0; i < m_vecChildrenWin.size(); i++)
	{
		m_vecChildrenWin[i]->Render();
	}

	return S_OK;
}

void CImGuiWin::Free()
{
	m_vecChildrenWin.clear();
}

HRESULT CImGuiWin::Add_Child(CImGuiWin* pWin)
{
	if (S_OK == Exists_OnChild(pWin))
		return E_FAIL;

	m_vecChildrenWin.push_back(pWin);

	return S_OK;
}

HRESULT CImGuiWin::Exists_OnChild(CImGuiWin* const pWin)
{
	if (m_vecChildrenWin.empty())
	{
		if (this == pWin)
			return S_OK;
		else
			return E_FAIL;
	}
	else
	{
		for (_uint i = 0; i < m_vecChildrenWin.size(); i++)
		{
			if (S_OK == m_vecChildrenWin[i]->Exists_OnChild(pWin))
				return S_OK;
		}
	}

	return E_FAIL;
}
