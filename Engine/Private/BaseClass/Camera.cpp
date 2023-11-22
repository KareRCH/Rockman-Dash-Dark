#include "BaseClass/Camera.h"

CCamera::CCamera()
    : m_vAt(_float3(0.f, 0.f, 0.f)), m_vUp(_float3(0.f, 1.f, 0.f))
    , m_matPersView(), m_matPersProj()
{
    XMStoreFloat4x4(&m_matPersView, XMMatrixIdentity());
    XMStoreFloat4x4(&m_matPersProj, XMMatrixIdentity());
}

CCamera::CCamera(const CCamera& rhs)
    : Base(rhs)
    , m_matPersView(rhs.m_matPersView)
    , m_matPersProj(rhs.m_matPersProj)
{
}

HRESULT CCamera::Initialize()
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

void CCamera::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    return 0;
}

void CCamera::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

void CCamera::Render()
{
    SUPER::Render();


}

void CCamera::Free()
{
    SUPER::Free();
}
