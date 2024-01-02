#include "ImGuiWin/ImGuiWin_Hierarchi.h"

#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "ImGuiWin/ImGuiWin_Browser.h"

#include "BaseClass/GameObject.h"

HRESULT CImGuiWin_Hierarchi::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Hierarchi::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	Link_ToBrowser();

	ImGui::Begin(u8"°èÃþ");

	Layout_ObjectList(fTimeDelta);

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Hierarchi::Render()
{
	return S_OK;
}

CImGuiWin_Hierarchi* CImGuiWin_Hierarchi::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CImGuiWin_Viewer Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_Hierarchi::Free()
{
	SUPER::Free();
}

void CImGuiWin_Hierarchi::Layout_ObjectList(const _float& fTimeDelta)
{
	if (ImGui::BeginListBox(u8"##°èÃþ"))
	{
		for (_uint i = 0; i < m_vecGameObjects.size(); i++)
		{
			string strName = "";
			strName.assign(m_vecGameObjects[i]->Get_Name().begin(), m_vecGameObjects[i]->Get_Name().end());
			if (ImGui::Selectable(strName.c_str(), m_iSelected_GameObject == i))
			{
				m_iSelected_GameObject = i;
			}
		}

		ImGui::EndListBox();
	}	
}

void CImGuiWin_Hierarchi::Handle_ObjectPlaced(CGameObject* pObj)
{
	if (pObj == nullptr)
		return;

	m_vecGameObjects.push_back(pObj);
}

void CImGuiWin_Hierarchi::Link_ToBrowser()
{
	if (m_bIsLinkedToBrowser || !m_pParentWin)
		return;

	CImGuiWin_Browser* pBrowser = { nullptr };
	m_pParentWin->Find_Child(&pBrowser);
	if (pBrowser != nullptr)
	{
		pBrowser->Add_ObjectPlacedListener(MakeDelegate(this, &ThisClass::Handle_ObjectPlaced));
		m_bIsLinkedToBrowser = true;
	}
}
