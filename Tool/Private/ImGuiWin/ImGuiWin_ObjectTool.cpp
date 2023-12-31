#include "ImGuiWin/ImGuiWin_ObjectTool.h"

HRESULT CImGuiWin_ObjectTool::Initialize()
{
	m_bOpen = false;

    return S_OK;
}

void CImGuiWin_ObjectTool::Tick(const _float& fTimeDelta)
{
	if (m_bOpen)
		return;


	SUPER::Tick(fTimeDelta);
}

HRESULT CImGuiWin_ObjectTool::Render()
{
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
