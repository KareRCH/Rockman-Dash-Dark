#include "ImGuiWin/ImGuiWin_Navigation.h"

CImGuiWin_Navigation::CImGuiWin_Navigation()
{
    NULL_CHECK(m_pGI = GI());
    Safe_AddRef(m_pGI);

    NULL_CHECK(m_pDeviceComp = Cast<CD3D11DeviceComp*>(m_pGI->Reference_PrototypeComp(L"GraphicDevComp")));
    NULL_CHECK(m_pPipelineComp = Cast<CPipelineComp*>(m_pGI->Reference_PrototypeComp(L"CamViewComp")));
}

HRESULT CImGuiWin_Navigation::Initialize()
{
	m_bOpen = true;

    return S_OK;
}

void CImGuiWin_Navigation::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	// 부모 창으로부터 뷰어하나에 대해 델리게이트를 연결
	//Link_ToViewer();

	ImGui::Begin(u8"네비게이션");

	if (ImGui::BeginTabBar(u8"터레인 모드"))
	{
		if (ImGui::BeginTabItem(u8"생성"))
		{
			m_eMode = EMode::Create;

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"편집"))
		{
			m_eMode = EMode::Edit;

			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	switch (m_eMode)
	{
	case EMode::Create:
		Layout_NavigationCreate(fTimeDelta);
		break;
	case EMode::Edit:
		Layout_NavgationiEdit(fTimeDelta);
		break;
	}

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Navigation::Render()
{
	SUPER::Render();

	return S_OK;
}

CImGuiWin_Navigation* CImGuiWin_Navigation::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ImGuiWin_Navigation Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGuiWin_Navigation::Free()
{
    SUPER::Free();

    Safe_Release(m_pDeviceComp);
    Safe_Release(m_pPipelineComp);
    Safe_Release(m_pGI);
}

void CImGuiWin_Navigation::Layout_NavigationCreate(const _float& fTimeDelta)
{
	ImGui::Text(u8"네비게이션 생성하기");
	if (ImGui::Button(u8"생성##Navigation"))
	{

	}
}

void CImGuiWin_Navigation::Layout_NavgationiEdit(const _float& fTimeDelta)
{
	ImGui::Text(u8"네비게이션 편집하기");
	if (ImGui::Button(u8"편집##Navigation"))
	{

	}
}
