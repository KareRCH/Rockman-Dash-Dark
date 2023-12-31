#include "ImGuiWin/ImGuiWin_MapTool.h"

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

    ImGuiWindowFlags iMain_Flags = ImGuiWindowFlags_NoMove;
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
            ImGuiID dock_terrain_id = ImGui::DockBuilderSplitNode(dock_hierarchi_id, ImGuiDir_Down, 0.5f, &dock_property_id, &dock_hierarchi_id);
            

            ImGui::DockBuilderDockWindow(u8"뷰어", dockspace_id);
            ImGui::DockBuilderDockWindow(u8"계층", dock_hierarchi_id);
            ImGui::DockBuilderDockWindow(u8"속성", dock_property_id);
            ImGui::DockBuilderDockWindow(u8"터레인", dock_terrain_id);

            ImGuiID dock_browser_scene = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 200.f / vDockSize.y, NULL, &dockspace_id);

            ImGui::DockBuilderDockWindow(u8"브라우저", dock_browser_scene);

            ImGui::DockBuilderFinish(dockspace_id);
        }
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    m_Actions.Update();
    Shortcut_Manage();
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


