#include "GameObject/DynamicCamera.h"

CDynamicCamera::CDynamicCamera(ID3D11Device* const pDevice)
    : Base(pDevice)
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

_int CDynamicCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);


    Set_Position(_float3(0.f, 0.f, 5.f));
    Calculate_Transform();

    _float3 vPos, vUp, vAt;
    vPos = Get_Position();
    vAt = _float3(0.f, 0.f, 10.f);
    vUp = Get_UpFromTransform();

    m_matPersView = XMMatrixLookAtLH(XMLoadFloat3(&vPos),
        XMLoadFloat3(&vAt), XMLoadFloat3(&vUp));
    m_matPersProj = XMMatrixPerspectiveFovLH(3.141592654f / 4.f, ((_float)g_iWindowSizeX / (_float)g_iWindowSizeY), 0.01f, 1000.f);
    //m_matPersProj = XMMatrixOrthographicLH(g_iWindowSizeX, g_iWindowSizeY, 0.1f, 1000.f);

    GameInstance()->Set_PerspectiveViewMatrix(0U, Get_Transform());
    GameInstance()->Set_PerspectiveProjMatrix(0U, m_matPersProj);

    return 0;
}

void CDynamicCamera::LateTick()
{
    SUPER::LateTick();
}

void CDynamicCamera::Render(ID3D11DeviceContext* const pDeviceContext)
{
    SUPER::Render(pDeviceContext);
}

CDynamicCamera* CDynamicCamera::Create(ID3D11Device* const pDevice)
{
    ThisClass* pInstance = new ThisClass(pDevice);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("DynamicCamera Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CDynamicCamera::Free()
{
    SUPER::Free();
}
