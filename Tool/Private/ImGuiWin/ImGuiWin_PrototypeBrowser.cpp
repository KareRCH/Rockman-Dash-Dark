#include "ImGuiWin/ImGuiWin_PrototypeBrowser.h"

#include "ImGuiWin/ImGuiWin_ClassBrowser.h"
#include "ImGuiWin/ImGuiWin_PrototypeProperty.h"
#include "Utility/RapidJsonSerial.h"
#include "GameObject/GameObjectFactory.h"
#include "BaseClass/GameObject.h"

HRESULT CImGuiWin_PrototypeBrowser::Initialize()
{
    m_bOpen = true;

    return S_OK;
}

void CImGuiWin_PrototypeBrowser::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;

    ImGui::Begin(u8"브라우저##ObjectTool");

    Layout_Object(fTimeDelta);

    SUPER::Tick(fTimeDelta);

    ImGui::End();
}

HRESULT CImGuiWin_PrototypeBrowser::Render()
{
    return S_OK;
}

CImGuiWin_PrototypeBrowser* CImGuiWin_PrototypeBrowser::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ImGuiWin_ObjectTool Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CImGuiWin_PrototypeBrowser::Free()
{
    SUPER::Free();
}

void CImGuiWin_PrototypeBrowser::Layout_Object(const _float& fTimeDelta)
{
	ImGui::Text(u8"오브젝트 리스트");
	if (ImGui::Button(u8"생성하기"))
	{
		Create_Object();
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

void CImGuiWin_PrototypeBrowser::Create_Object()
{
	if (m_iSelected_Object == -1)
		return;

	GI()->Set_LevelTag(TEXT("ObjectTool"));

	FSerialData Data;
	Data.Load_Data(GI()->Get_MainPath() + TEXT("Resource/Prototypes/") + ConvertToWstring(m_vecPrototypeNames[m_iSelected_Object]));

	CGameObject* pAddedObject = { nullptr };
	GI()->Add_GameObject(pAddedObject = CGameObjectFactory::Create(Data));

	if (pAddedObject == nullptr)
		return;

	pAddedObject->TurnOff_State(EGObjectState::Tick);

	CImGuiWin_PrototypeProperty* pWin = { nullptr };
	m_pParentWin->Find_Child<CImGuiWin_PrototypeProperty>(&pWin);
	if (nullptr != pWin)
	{
		pWin->Set_GameObject(pAddedObject);
	}

	CImGuiWin_ClassBrowser* pClassWin = { nullptr };
	m_pParentWin->Find_Child<CImGuiWin_ClassBrowser>(&pClassWin);
	if (nullptr != pClassWin)
	{
		pClassWin->Set_GameObject(pAddedObject);
	}
}
