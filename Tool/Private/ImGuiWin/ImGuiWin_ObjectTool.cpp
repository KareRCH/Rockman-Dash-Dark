#include "ImGuiWin/ImGuiWin_ObjectTool.h"

#include "ImGuiWin/ImGuiWin_ClassBrowser.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"
#include "dirent/dirent.h"

HRESULT CImGuiWin_ObjectTool::Initialize()
{
	m_bOpen = false;
    m_bFirstLoop = true;

    return S_OK;
}

void CImGuiWin_ObjectTool::Tick(const _float& fTimeDelta)
{
    if (!m_bOpen)
        return;

    ImGuiWindowFlags iMain_Flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar;
    ImGuiDockNodeFlags DockSpace_Flags = ImGuiDockNodeFlags_NoDockingInCentralNode;

    ImGui::Begin("Center", NULL, iMain_Flags);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("DockSpace_ObjectTool");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), DockSpace_Flags);

        if (m_bFirstLoop)
        {
            m_bFirstLoop = false;
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, DockSpace_Flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());
            ImVec2 vDockSize = ImGui::GetWindowSize();

            ImGuiID dock_browser_scene = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 200.f / vDockSize.y, NULL, &dockspace_id);
            ImGuiID dock_hierarchi_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 250.f / vDockSize.x, NULL, &dockspace_id);
            ImGuiID dock_property_id = ImGui::DockBuilderSplitNode(dock_hierarchi_id, ImGuiDir_Down, 0.5f, NULL, &dock_hierarchi_id);

            ImGui::DockBuilderDockWindow(u8"뷰어##ObjectTool", dockspace_id);
            ImGui::DockBuilderDockWindow(u8"계층##ObjectTool", dock_hierarchi_id);
            ImGui::DockBuilderDockWindow(u8"속성##ObjectTool", dock_property_id);

            ImGui::DockBuilderDockWindow(u8"클래스##ObjectTool", dock_browser_scene);
            ImGui::DockBuilderDockWindow(u8"브라우저##ObjectTool", dock_browser_scene);

            ImGui::DockBuilderFinish(dockspace_id);
        }
    }
    else
    {
        //ShowDockingDisabledMessage();
    }

    Layout_MenuBar(fTimeDelta);

    SUPER::Tick(fTimeDelta);

    ImGui::End();
}

HRESULT CImGuiWin_ObjectTool::Render()
{
    SUPER::Render();

    return S_OK;
}

CImGuiWin_ObjectTool* CImGuiWin_ObjectTool::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiWin_ObjectTool Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_ObjectTool::Free()
{
	SUPER::Free();
}

void CImGuiWin_ObjectTool::Layout_MenuBar(const _float& fTimeDelta)
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Prototype"))
        {
            if (ImGui::MenuItem("Open"))
            {

            }

            if (ImGui::MenuItem("Save"))
            {
                Show_SavePrototype();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    Save_Prototype();
}

void CImGuiWin_ObjectTool::Show_SavePrototype()
{
    ImGuiFileDialog::Instance()->OpenDialog("PrototypeSave", "Save Prototype", ".aproto", "../Client/Resource/Prototypes/");
}

void CImGuiWin_ObjectTool::Save_Prototype()
{
    if (ImGuiFileDialog::Instance()->Display("PrototypeSave"))
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
            CImGuiWin_ClassBrowser* pClassBrowser = { nullptr };
            Find_Child<CImGuiWin_ClassBrowser>(&pClassBrowser);

            if (pClassBrowser)
            {
                pClassBrowser->Save_Prototype(ConvertToWstring(filePathName));
            }
        }
        // close
        ImGuiFileDialog::Instance()->Close();
    }
}
