#include "ImGuiWin/ImGuiWin_Browser.h"

#include "ImGuiWin/ImGuiWin_MapTool.h"
#include "ImGuiWin/ImGuiWin_Viewer.h"
#include "ImGuiWin/ImGuiWin_Terrain.h"
#include "ImGuiWin/ImGuiWin_Property.h"

#include "GameObject/Player.h"
#include "GameObject/ReaverBot_Horokko.h"
#include "GameObject/ItemChest.h"

#include "GameObject/GameObjectFactory.h"


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

	ImGuiWindowFlags iMain_Flags = ImGuiWindowFlags_NoMove;

	ImGui::Begin(u8"브라우저", NULL, iMain_Flags);

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

	ImGui::SameLine();
	if (ImGui::Button(u8"프로토타입 로드하기"))
	{
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = FindFirstFile((GI()->Get_MainPath() + TEXT("Resource/Prototypes/*.aproto")).c_str(), &findFileData);

		if (hFind == INVALID_HANDLE_VALUE) {
			std::cerr << "파일찾기 에러" << std::endl;
		}
		else
		{
			m_vecPrototypeNames.clear();
			do {
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					// 디렉터리 무시
				}
				else {
					// 파일 이름 출력
					m_vecPrototypeNames.push_back(ConvertToString(wstring(findFileData.cFileName)));
				}
			} while (FindNextFile(hFind, &findFileData) != 0);
		}

		FindClose(hFind);
	}

	if (ImGui::BeginListBox(u8"##오브젝트"))
	{
		ImGuiSelectableFlags eFlag = {};
		eFlag = ImGuiSelectableFlags_None;
		
		for (_uint i = 0; i < m_vecPrototypeNames.size(); i++)
		{
			_char szName[MAX_PATH] = {};
			strcpy_s(szName, m_vecPrototypeNames[i].c_str());
			if (ImGui::Selectable(szName, (m_iSelected_Object == i), eFlag))
			{
				m_iSelected_Object = i;
			}
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
	if (m_iSelected_Object == -1)
		return;

	GI()->Set_LevelTag(TEXT("MapTool"));

	FSerialData Data;
	Data.Load_Data(GI()->Get_MainPath()  + g_PrototypePath + ConvertToWstring(m_vecPrototypeNames[m_iSelected_Object]));

	Data.Add_Member("PosX", vPickedWorldPos.x);
	Data.Add_Member("PosY", vPickedWorldPos.y);
	Data.Add_Member("PosZ", vPickedWorldPos.z);

	CGameObject* pAddedObject = { nullptr };
	GI()->Add_GameObject(pAddedObject = CGameObjectFactory::Create(Data));

	if (pAddedObject == nullptr)
		return;

	pAddedObject->TurnOff_State(EGObjectState::Tick);

	CImGuiWin_Property* pWinProperty = { nullptr };
	m_pParentWin->Find_Child<CImGuiWin_Property>(&pWinProperty);

	if (pWinProperty)
		pWinProperty->Set_GameObject(pAddedObject);

	OnObjectPlaced.Broadcast(pAddedObject);
}
