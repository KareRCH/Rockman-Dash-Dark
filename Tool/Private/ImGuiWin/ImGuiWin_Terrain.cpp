#include "ImGuiWin/ImGuiWin_Terrain.h"

#include "BaseClass/Terrain.h"

HRESULT CImGuiWin_Terrain::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Terrain::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	ImGui::Begin(u8"터레인");

	Layout_TerrainSetting(fTimeDelta);

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Terrain::Render()
{
	SUPER::Render();

	return S_OK;
}

CImGuiWin_Terrain* CImGuiWin_Terrain::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiWin_Terrain::Free()
{
	SUPER::Free();

	Safe_Release(m_pTerrain);
}

void CImGuiWin_Terrain::Layout_TerrainSetting(const _float& fTimeDelta)
{
	_bool	bIsChanged = false;

	ImGui::Text(u8"정점 개수");

	ImGui::Button(u8"X##Terrrain VertexX Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexX", &m_ivTerrainVertex_CountX, 0, 0, ImGuiInputTextFlags_None))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();
	
	ImGui::SameLine();

	ImGui::Button(u8"Y##Terrrain VertexY Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain VertexY", &m_ivTerrainVertex_CountY, 0, 0, ImGuiInputTextFlags_None))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();

	// 더미
	ImGui::Dummy(ImVec2(0.f, 30.f));




	// 터레인 크기
	ImGui::Text(u8"크기");

	ImGui::Button(u8"X##Terrrain Width Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Width", &m_ivTerrainWidth, 0, 0, ImGuiInputTextFlags_None))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();


	ImGui::SameLine();


	ImGui::Button(u8"Y##Terrrain Height Button");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 0.0f));
	ImGui::SameLine();
	ImGui::PopStyleVar();
	ImGui::PushItemWidth(80.f);
	if (ImGui::InputInt(u8"##Terrain Height", &m_ivTerrainHeight, 0, 0, ImGuiInputTextFlags_None))
	{
		bIsChanged = true;
	}
	ImGui::PopItemWidth();


	// 변경 사항을 적용한다.
	if (bIsChanged)
	{
		// 만들어진 터레인이 없다면 만들어 추가하고, 
		if (nullptr == m_pTerrain)
		{
			GI()->Add_GameObject(m_pTerrain = CTerrain::Create());
			Safe_AddRef(m_pTerrain);

			//m_pTerrain->
		}
		else
		{
			// 여기서 터레인의 사이즈를 다시 조절한다.
			

			//m_pTerrain
		}
	}
}
