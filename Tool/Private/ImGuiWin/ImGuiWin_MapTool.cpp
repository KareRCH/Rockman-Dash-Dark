#include "ImGuiWin/ImGuiWin_MapTool.h"

#include "Utility/RapidJsonSerial.h"
#include "BaseClass/GameObject.h"
#include "GameObject/GameObjectFactory.h"
#include "ImGuiFileDialog.h"
#include "ImGuiWin/ImGuiWin_Hierarchi.h"

HRESULT CImGuiWin_MapTool::Initialize()
{
	m_bOpen = true;
	m_bFirstLoop = true;

    m_pPickedObjects.reserve(100);
    m_Actions.Add_Action(EAction::CancelMove);
    m_Actions.Add_Action(EAction::ConfirmMove);

	return S_OK;
}

void CImGuiWin_MapTool::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;

    ImGuiWindowFlags iMain_Flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;
    ImGuiDockNodeFlags DockSpace_Flags = ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGui::Begin("Center", NULL, iMain_Flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("DockSpace_MapTool");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), DockSpace_Flags);

        if (m_bFirstLoop)
        {
            m_bFirstLoop = false;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, DockSpace_Flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());
            ImVec2 vDockSize = ImGui::GetWindowSize();

            ImGuiID dock_hierarchi_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 250.f / vDockSize.x, NULL, &dockspace_id);
            ImGuiID dock_property_id = ImGui::DockBuilderSplitNode(dock_hierarchi_id, ImGuiDir_Down, 0.5f, NULL, &dock_hierarchi_id);
            
            ImGui::DockBuilderDockWindow(u8"뷰어", dockspace_id);
            ImGui::DockBuilderDockWindow(u8"계층", dock_hierarchi_id);
            ImGui::DockBuilderDockWindow(u8"속성", dock_property_id);
            ImGui::DockBuilderDockWindow(u8"터레인", dock_property_id);
            ImGui::DockBuilderDockWindow(u8"네비게이션", dock_property_id);

            ImGuiID dock_browser_scene = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 200.f / vDockSize.y, NULL, &dockspace_id);

            ImGui::DockBuilderDockWindow(u8"브라우저", dock_browser_scene);

            ImGui::DockBuilderFinish(dockspace_id);
        }
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    m_Actions.Reset();
    Shortcut_Manage();

    Layout_MenuBar(fTimeDelta);
    DoMove_PickedObjects(fTimeDelta);

    SUPER::Tick(fTimeDelta);


    ImGui::End();
}

HRESULT CImGuiWin_MapTool::Render()
{
    SUPER::Render();

	return S_OK;
}

CImGuiWin_MapTool* CImGuiWin_MapTool::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_MapTool::Free()
{
	SUPER::Free();
}

void CImGuiWin_MapTool::Layout_MenuBar(const _float& fTimeDelta)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Level"))
        {
            if (ImGui::MenuItem("Open"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("LevelLoad", "Load Level", ".alevel", "../Client/Resource/Levels/");
            }

            if (ImGui::MenuItem("Save"))
            {
                ImGuiFileDialog::Instance()->OpenDialog("LevelSave", "Save Level", ".alevel", "../Client/Resource/Levels/");
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    if (ImGuiFileDialog::Instance()->Display("LevelLoad"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
            // here convert from string because a string was passed as a userDatas, but it can be what you want
            std::string userDatas;
            if (ImGuiFileDialog::Instance()->GetUserDatas())
                userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
            auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

            // action
            Load_Level(ConvertToWstring(filePathName));
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGuiFileDialog::Instance()->Display("LevelSave"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            std::string filter = ImGuiFileDialog::Instance()->GetCurrentFilter();
            // here convert from string because a string was passed as a userDatas, but it can be what you want
            std::string userDatas;
            if (ImGuiFileDialog::Instance()->GetUserDatas())
                userDatas = std::string((const char*)ImGuiFileDialog::Instance()->GetUserDatas());
            auto selection = ImGuiFileDialog::Instance()->GetSelection(); // multiselection

            // action
            Save_Level(ConvertToWstring(filePathName));
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}

void CImGuiWin_MapTool::Shortcut_Manage()
{
    // 행동 취소, 모드 돌아오기
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        m_Actions[EAction::CancelMove].Act();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Tab))
    {
        if (m_eToolMode == EMode::Picking)
        {
            m_eToolMode = EMode::Place;
            Escape_MovePickedObjects();
        }
        else if (m_eToolMode == EMode::Place)
            m_eToolMode = EMode::Picking;
    }

    // 피킹 된게 없으면 MoveMode를 초기화
    if (m_pPickedObjects.empty())
    {
        m_eMoveMode = EMoveMode::None;
    }

    // 이동관련 단축키
    if (m_eToolMode == EMode::Picking && !m_pPickedObjects.empty())
    {
        // 이동 
        if (ImGui::IsKeyPressed(ImGuiKey_G))
        {
            m_eMoveMode = EMoveMode::Move;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_R))
        {
            m_eMoveMode = EMoveMode::Rotate;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_S))
        {
            m_eMoveMode = EMoveMode::Scale;
        }

        // 편집 축 선택
        if (ImGui::IsKeyPressed(ImGuiKey_X))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::YZ;
            else
                m_eMoveAxis = EMoveAxis::X;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Y))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::ZX;
            else
                m_eMoveAxis = EMoveAxis::Y;
        }
        else if (ImGui::IsKeyPressed(ImGuiKey_Z))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
                m_eMoveAxis = EMoveAxis::XY;
            else
                m_eMoveAxis = EMoveAxis::Z;
        }
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Enter))
    {
        m_Actions[EAction::ConfirmMove].Act();
    }
}

void CImGuiWin_MapTool::DoMove_PickedObjects(const _float& fTimeDelta)
{
    if (m_pPickedObjects.empty())
        return;

    switch (m_eMoveMode)
    {
    case EMoveMode::Move:
        Move_PickedObjects(fTimeDelta);
        break;
    case EMoveMode::Rotate:
        Rotate_PickedObjects(fTimeDelta);
        break;
    case EMoveMode::Scale:
        Scale_PickedObjects(fTimeDelta);
        break;
    default:
        break;
    }
}

void CImGuiWin_MapTool::Move_PickedObjects(const _float& fTimeDelta)
{
    for (_uint i = 0; i < m_pPickedObjects.size(); i++)
    {
        switch (m_eMoveAxis)
        {
        case EMoveAxis::X:

            break;
        case EMoveAxis::Y:

            break;
        case EMoveAxis::Z:

            break;
        case EMoveAxis::XY:

            break;
        case EMoveAxis::YZ:

            break;
        case EMoveAxis::ZX:

            break;
        case EMoveAxis::ALL:

            break;
        default:
            break;
        }
    }
}

void CImGuiWin_MapTool::Rotate_PickedObjects(const _float& fTimeDelta)
{
    for (_uint i = 0; i < m_pPickedObjects.size(); i++)
    {
        switch (m_eMoveAxis)
        {
        case EMoveAxis::X:

            break;
        case EMoveAxis::Y:

            break;
        case EMoveAxis::Z:

            break;
        case EMoveAxis::XY:

            break;
        case EMoveAxis::YZ:

            break;
        case EMoveAxis::ZX:

            break;
        case EMoveAxis::ALL:

            break;
        default:
            break;
        }
    }
}

void CImGuiWin_MapTool::Scale_PickedObjects(const _float& fTimeDelta)
{
    for (_uint i = 0; i < m_pPickedObjects.size(); i++)
    {
        switch (m_eMoveAxis)
        {
        case EMoveAxis::X:

            break;
        case EMoveAxis::Y:

            break;
        case EMoveAxis::Z:

            break;
        case EMoveAxis::XY:

            break;
        case EMoveAxis::YZ:

            break;
        case EMoveAxis::ZX:

            break;
        case EMoveAxis::ALL:

            break;
        default:
            break;
        }
    }
}

void CImGuiWin_MapTool::Escape_MovePickedObjects()
{
    // 이동 편집 중이던 작업 취소
}

void CImGuiWin_MapTool::Load_Level(const wstring& strLoadPath)
{
    FSerialData LevelData;
    LevelData.Load_Data(strLoadPath);

    FSerialData ObjectDatas;
    LevelData.Get_Data("CloneObjects", ObjectDatas);

    list<CGameObject*> listLoadedGameObjects;

    _uint iNumObjects = ObjectDatas.Get_ArraySize("Objects");
    for (_uint i = 0; i < iNumObjects; i++)
    {
        FSerialData ObjectData;
        ObjectDatas.Get_ObjectFromArray("Objects", i, ObjectData);

        /*StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        ObjectData.m_Doc.Accept(writer);

        cout << buffer.GetString() << endl;*/

        _uint iClassID = 0;
        if (FAILED(ObjectData.Get_Data("ClassID", iClassID)))
            return;

        CGameObject* pObj = CGameObjectFactory::Create(ObjectData);
        if (pObj == nullptr)
            return;

        listLoadedGameObjects.push_back(pObj);
    }

    // 검증 되었으면 추가한다.
    GI()->Clear_GameObject(TEXT("MapTool"));
    GI()->Set_LevelTag(TEXT("MapTool"));
    CImGuiWin_Hierarchi* pWinHierarchi = { nullptr };
    Find_Child<CImGuiWin_Hierarchi>(&pWinHierarchi);
    if (pWinHierarchi)
        pWinHierarchi->Reset_GameObjectList();
    for (auto iter = listLoadedGameObjects.begin(); iter != listLoadedGameObjects.end(); ++iter)
    {
        GI()->Add_GameObject((*iter));
        (*iter)->TurnOff_State(EGObjectState::Tick);

        if (pWinHierarchi)
            pWinHierarchi->Pushback_GameObject((*iter));
    }
}

void CImGuiWin_MapTool::Save_Level(const wstring& strSavePath)
{
    auto vecGameObjects = GI()->Get_AllGameObjectFromLevel(TEXT("MapTool"));

    FSerialData LevelData;
    LevelData.Add_MemberString("Name", "GamePlay");

    FSerialData PrototypeData;
    
    for (size_t i = 0; i < vecGameObjects.size(); i++)
    {
        auto ObjData = vecGameObjects[i]->SerializeData_Prototype();

        _uint iNumComponents = ObjData.Get_ArraySize("Components");
        for (_uint j = 0; j < iNumComponents; j++)
        {
            FSerialData ComponentData;
            ObjData.Get_ObjectFromArray("Components", j, ComponentData);
            PrototypeData.Pushback_Member("Components", ComponentData);
        }

        PrototypeData.Pushback_Member("Objects", ObjData);
    }
    LevelData.Add_Member("Prototypes", PrototypeData);

    FSerialData CloneData;
    for (size_t i = 0; i < vecGameObjects.size(); i++)
    {
        auto ObjData = vecGameObjects[i]->SerializeData();
        CloneData.Pushback_Member("Objects", ObjData);
    }
    LevelData.Add_Member("CloneObjects", CloneData);

    LevelData.Save_Data(strSavePath);
}


