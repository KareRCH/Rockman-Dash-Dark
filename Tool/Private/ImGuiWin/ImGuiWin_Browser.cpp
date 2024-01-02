#include "ImGuiWin/ImGuiWin_Browser.h"

#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Terrain.h"

#include "GameObject/Player.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/ItemChest.h"

HRESULT CImGuiWin_Browser::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Browser::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	Link_ToMapTool();
	Link_ToTerrainGui();
	Link_ToViewer();

	ImGui::Begin(u8"브라우저");

	Layout_Object(fTimeDelta);

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Browser::Render()
{
	return S_OK;
}

CImGuiWin_Browser* CImGuiWin_Browser::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CImGuiWin_Browser Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_Browser::Free()
{
	SUPER::Free();
}

void CImGuiWin_Browser::Layout_Object(const _float& fTimeDelta)
{
	ImGui::Text(u8"오브젝트 리스트");
	if (ImGui::Button(u8"배치모드"))
	{
		if (m_ePlaceMode == EMode::None)
		{
			m_ePlaceMode = EMode::Place;
			OnPlaceModeSelected.Broadcast(true);
		}
	}

	if (ImGui::BeginListBox(u8"##오브젝트"))
	{
		ImGuiSelectableFlags eFlag = {};
		eFlag = ImGuiSelectableFlags_None;
		

		if (ImGui::Selectable(u8"TestObject", (m_iSelected_Object == 0), eFlag))
		{
			m_iSelected_Object = 0;
		}

		ImGui::EndListBox();
	}	
}

void CImGuiWin_Browser::Link_ToMapTool()
{
	if (m_bIsLinkedToMapTool || !m_pParentWin)
		return;

	CImGuiWin_MapTool* pMapTool = { nullptr };
	m_pParentWin->Find_Child(&pMapTool);
	if (pMapTool != nullptr)
	{
		//pMapTool->Add(MakeDelegate(this, &ThisClass::Handle_ModeSelected));
		m_bIsLinkedToMapTool = true;
	}
}

void CImGuiWin_Browser::Link_ToTerrainGui()
{
	if (m_bIsLinkedToTerrainGui || !m_pParentWin)
		return;

	CImGuiWin_Terrain* pTerrainGui = { nullptr };
	m_pParentWin->Find_Child(&pTerrainGui);
	if (pTerrainGui != nullptr)
	{
		pTerrainGui->Add_ModeSelectedListener(MakeDelegate(this, &ThisClass::Handle_TerrainBrushModeSelected));
		m_bIsLinkedToTerrainGui = true;
	}
}

void CImGuiWin_Browser::Link_ToViewer()
{
	if (m_bIsLinkedToViewer || !m_pParentWin)
		return;

	CImGuiWin_Viewer* pViewer = { nullptr };
	m_pParentWin->Find_Child(&pViewer);
	if (pViewer != nullptr)
	{
		pViewer->Add_PickedPlaceListener(MakeDelegate(this, &ThisClass::Handle_PlacePicked));
		m_bIsLinkedToViewer = true;
	}
}

void CImGuiWin_Browser::Handle_TerrainBrushModeSelected(_bool bIsEdit)
{
	// 터레인에서 수정할거라는 신호가 옴
	if (bIsEdit)
		m_ePlaceMode = EMode::None;
}

void CImGuiWin_Browser::Handle_PlacePicked(_float3 vPickedWorldPos)
{
	CGameObject* pAddedObject = { nullptr };
	switch (m_iSelected_Object)
	{
	case 0:
		GI()->Add_GameObject(pAddedObject = CPlayer::Create(vPickedWorldPos));
		break;
	}

	if (pAddedObject == nullptr)
		return;

	pAddedObject->TurnOff_State(EGObjectState::Tick);
	
	OnObjectPlaced.Broadcast(pAddedObject);
}
