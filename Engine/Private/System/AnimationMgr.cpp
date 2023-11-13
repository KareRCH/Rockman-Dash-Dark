#include "System/AnimationMgr.h"

CAnimationMgr::CAnimationMgr(const DX11DEVICE_T tDevice)
    : m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CAnimationMgr::Initialize()
{
    return S_OK;
}

CAnimationMgr* CAnimationMgr::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("RenderMgr Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CAnimationMgr::Free()
{
}
