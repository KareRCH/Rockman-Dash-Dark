#include "GameObject/DynamicCamera.h"

CDynamicCamera::CDynamicCamera(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
    Set_Name(L"DynamicCamera");
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
    : Base(rhs)
{
}

HRESULT CDynamicCamera::Initialize()
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

void CDynamicCamera::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

_int CDynamicCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);


    Set_Position(_float3(6.f, 6.f, 6.f));

    _float3 vPos, vUp, vAt;
    vPos = Get_Position();
    vAt = _float3(0.f, 0.f, 0.f);
    vUp = _float3(0.f, 1.f, 0.f);

    m_matPersView = XMMatrixLookAtLH(XMLoadFloat3(&vPos),
        XMLoadFloat3(&vAt), XMLoadFloat3(&vUp));
    m_matPersProj = XMMatrixPerspectiveFovLH(XM_PI * 0.25f, ((_float)g_iWindowSizeX / (_float)g_iWindowSizeY), 0.1f, 1000.f);
    //m_matPersProj = XMMatrixOrthographicLH(g_iWindowSizeX, g_iWindowSizeY, 0.1f, 1000.f);

    GameInstance()->Set_PerspectiveViewMatrix(0U, m_matPersView);
    GameInstance()->Set_PerspectiveProjMatrix(0U, m_matPersProj);

    return 0;
}

void CDynamicCamera::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

void CDynamicCamera::Render()
{
    SUPER::Render();
}

CDynamicCamera* CDynamicCamera::Create(const DX11DEVICE_T tDevice)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("DynamicCamera Create Failed");

        return nullptr;
    }

    return pInstance;
}

CGameObject* CDynamicCamera::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("DynamicCamera Create Failed");

        return nullptr;
    }

    /* Arg Ã³¸® */

    return Cast<CGameObject*>(pInstance);
}

void CDynamicCamera::Free()
{
    SUPER::Free();
}
