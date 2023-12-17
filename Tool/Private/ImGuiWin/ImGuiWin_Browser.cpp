#include "ImGuiWin/ImGuiWin_Browser.h"

HRESULT CImGuiWin_Browser::Initialize()
{
	m_bOpen = true;

	return S_OK;
}

void CImGuiWin_Browser::Tick(const _float& fTimeDelta)
{
	if (!m_bOpen)
		return;

	ImGui::Begin(u8"������");

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
	ImGui::Text(u8"������Ʈ ����Ʈ");
	if (ImGui::Button(u8"��ġ���"))
	{

	}
}
