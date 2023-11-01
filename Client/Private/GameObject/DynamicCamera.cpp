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


    Set_Position(_float3(0.f, 0.f, -10.f));
    Calculate_Transform();

    GameInstance()->Set_PerspectiveViewMatrix(0U, Get_Transform());

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
