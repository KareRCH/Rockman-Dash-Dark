#include "ImGuiWin/ImGuiWin_Hierarchi.h"

HRESULT CImGuiWin_Hierarchi::Initialize()
{
	return S_OK;
}

void CImGuiWin_Hierarchi::Tick(const _float& fTimeDelta)
{
	ImGui::Begin(u8"°èÃþ");

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Hierarchi::Render()
{
	return S_OK;
}

CImGuiWin_Hierarchi* CImGuiWin_Hierarchi::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CImGuiWin_Viewer Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiWin_Hierarchi::Free()
{
	SUPER::Free();
}
