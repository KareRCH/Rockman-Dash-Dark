#include "BaseClass/Camera.h"

CCamera::CCamera(ID3D11Device* const pDevice)
    : Base(pDevice)
{
}

CCamera::CCamera(const CCamera& rhs)
    : Base(rhs)
{
}

HRESULT CCamera::Initialize()
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

_int CCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    return 0;
}

void CCamera::LateTick()
{
    SUPER::LateTick();

}

void CCamera::Render(ID3D11DeviceContext* const pDeviceContext)
{
    SUPER::Render(pDeviceContext);


}

void CCamera::Free()
{
    SUPER::Free();
}
