#include "System/ImGuiMgr.h"

HRESULT CImGuiMgr::Initialize()
{
    return S_OK;
}

CImGuiMgr* CImGuiMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("ImGuiMgr Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CImGuiMgr::Free()
{
}
