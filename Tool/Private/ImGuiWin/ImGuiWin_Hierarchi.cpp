#include "ImGuiWin/ImGuiWin_Hierarchi.h"

#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "ImGuiWin/ImGuiWin_Browser.h"
#include "ImGuiWin/ImGuiWin_Property.h"

#include "BaseClass/GameObject.h"
#include "GameObject/GameObjectFactory.h"

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

				CImGuiWin_Property* pWinProperty = { nullptr };
				m_pParentWin->Find_Child<CImGuiWin_Property>(&pWinProperty);
				if (pWinProperty)
					pWinProperty->Set_GameObject(m_vecGameObjects[i]);
			}
		}

		Delete_GameObject();
		Clone_GameObject();

		ImGui::EndListBox();
	}
}

void CImGuiWin_Hierarchi::Pushback_GameObject(CGameObject* pObj)
{
	if (nullptr == pObj)
		return;

	m_vecGameObjects.push_back(pObj);
}

void CImGuiWin_Hierarchi::Reset_GameObjectList()
{
	m_vecGameObjects.clear();
}

void CImGuiWin_Hierarchi::Delete_GameObject()
{
	if (m_iSelected_GameObject == -1)
		return;

	if (ImGui::IsKeyPressed(ImGuiKey_Delete))
	{
		auto iter = m_vecGameObjects.begin() + m_iSelected_GameObject;
		if ((*iter) != nullptr)
		{
			CImGuiWin_Property* pWinProperty = { nullptr };
			m_pParentWin->Find_Child<CImGuiWin_Property>(&pWinProperty);
			if (pWinProperty != nullptr)
			{
				pWinProperty->Reset_GameObject();
			}

			m_vecGameObjects[m_iSelected_GameObject]->Set_Dead();
			m_iSelected_GameObject = -1;
			m_vecGameObjects.erase(iter);
		}
	}
}

void CImGuiWin_Hierarchi::Clone_GameObject()
{
	if (m_iSelected_GameObject == -1)
		return;

	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_D))
	{
		auto iter = m_vecGameObjects.begin() + m_iSelected_GameObject;
		if ((*iter) != nullptr)
		{
			CGameObject* pObj = CGameObjectFactory::Create((*iter)->SerializeData());
			if (nullptr == pObj)
				return;

			GI()->Add_GameObject(pObj);
			Pushback_GameObject(pObj);

			CImGuiWin_Property* pWinProperty = { nullptr };
			m_pParentWin->Find_Child<CImGuiWin_Property>(&pWinProperty);
			if (pWinProperty != nullptr)
				pWinProperty->Set_GameObject(pObj);

			m_iSelected_GameObject += 1;
		}
	}
}

void CImGuiWin_Hierarchi::Select_GameObject(CGameObject* pObj)
{
	if (nullptr == pObj)
		return;

	for (_uint i = 0; i < m_vecGameObjects.size(); i++)
	{
		if (m_vecGameObjects[i] == pObj)
			m_iSelected_GameObject = i;
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
