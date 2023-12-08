#include "ImGuiWin/ImGuiWin_Property.h"

HRESULT CImGuiWin_Property::Initialize()
{
	return S_OK;
}

void CImGuiWin_Property::Tick(const _float& fTimeDelta)
{
	ImGui::Begin(u8"¼Ó¼º");

	SUPER::Tick(fTimeDelta);

	ImGui::End();
}

HRESULT CImGuiWin_Property::Render()
{
	return S_OK;
}

CImGuiWin_Property* CImGuiWin_Property::Create()
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

void CImGuiWin_Property::Free()
{
	SUPER::Free();
}
