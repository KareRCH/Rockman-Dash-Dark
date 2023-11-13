#include "System/ParticleMgr.h"

CParticleMgr::CParticleMgr(const DX11DEVICE_T tDevice)
    : m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CParticleMgr::Initialize()
{
    return S_OK;
}

CParticleMgr* CParticleMgr::Create(const DX11DEVICE_T tDevice)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ParticleMgr Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CParticleMgr::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pDeviceContext);
}
