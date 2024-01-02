#include "ImGuiWin/ImGuiWin_Docking.h"

HRESULT CImGuiWin_Docking::Initialize()
{
	m_bOpen = true;
	m_bFirstLoop = true;

    return S_OK;
}

void CImGuiWin_Docking::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	m_DockSpace_Flags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking;
	m_Window_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse;

	if (m_bOpt_FullScreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		m_Window_Flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		m_Window_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.11f, 0.11f, 0.11f, 1.0f));
	}
	else
	{
		m_DockSpace_Flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	// 독스페이스의 뒤편이 렌더되도록 만드는 플래그, 안쓸거임
	if (m_DockSpace_Flags & ImGuiDockNodeFlags_PassthruCentralNode)
		m_Window_Flags |= ImGuiWindowFlags_NoBackground;

	if (!m_bOpt_Padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", &m_bOpen, m_Window_Flags);
	if (!m_bOpt_Padding)
		ImGui::PopStyleVar();

	if (m_bOpt_FullScreen)
	{
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(1);
	}

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{		
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_DockSpace_Flags);
	}
	else
	{
		//ShowDockingDisabledMessage();
	}

	if (m_bFirstLoop)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, m_DockSpace_Flags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetWindowSize());


		ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, NULL, &dockspace_id);
		ImGui::DockBuilderDockWindow("Left", dock_right_id);
		ImGui::DockBuilderDockWindow("Center", dockspace_id);
		//ImGui::DockBuilderDockWindow("One", dockspace_id);
		//ImGui::DockBuilderDockWindow("Two", dockspace_id);

		ImGui::DockBuilderFinish(dockspace_id);

		m_bFirstLoop = false;
	}

	MenuBar(fTimeDelta);

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Docking::Render()
{
	SUPER::Render();

    return S_OK;
}

CImGuiWin_Docking* CImGuiWin_Docking::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_Docking::Free()
{
	SUPER::Free();
}

void CImGuiWin_Docking::MenuBar(const _float& fTimeDelta)
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Open"))
			{

			}

			if (ImGui::MenuItem("Save"))
			{

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Export Datas To Client"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Program"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				PostQuitMessage(0);
			}

			ImGui::EndMenu();
		}

		ImGui::Dummy(ImVec2(50.f, 0.f));

		if (ImGui::BeginTabBar("Layout"))
		{
			if (ImGui::BeginTabItem(u8"맵 툴"))
			{				
				Open_JustOneChild(0);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"오브젝트 툴"))
			{
				Open_JustOneChild(1);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"애니메이션 툴"))
			{
				Open_JustOneChild(2);

				ImGui::EndTabItem();
			}


			ImGui::EndTabBar();
		}

		ImGui::EndMenuBar();
	}
}
