#include "GameObject/DynamicCamera.h"

CDynamicCamera::CDynamicCamera()
{
    Set_Name(L"DynamicCamera");
}

CDynamicCamera::CDynamicCamera(const CDynamicCamera& rhs)
    : Base(rhs)
{
}

HRESULT CDynamicCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CDynamicCamera::Initialize(void* Arg)
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

    Transform().Set_Position(_float3(6.f, 6.f, 6.f));

    _float3 vPos, vUp, vAt;
    vPos = Transform().Get_PositionFloat3();
    vAt = _float3(0.f, 0.f, 0.f);
    vUp = _float3(0.f, 1.f, 0.f);

    _matrix matPersView, matPersProj;

    matPersView = XMMatrixLookAtLH(XMLoadFloat3(&vPos),
        XMLoadFloat3(&vAt), XMLoadFloat3(&vUp));
    matPersProj = XMMatrixPerspectiveFovLH(XM_PI * 0.25f, ((_float)g_iWindowSizeX / (_float)g_iWindowSizeY), 0.1f, 1000.f);
    //m_matPersProj = XMMatrixOrthographicLH(g_iWindowSizeX, g_iWindowSizeY, 0.1f, 1000.f);

    XMStoreFloat4x4(&m_matPersView, matPersView);
    XMStoreFloat4x4(&m_matPersProj, matPersProj);

    GameInstance()->Set_PerspectiveViewMatrix(0U, matPersView);
    GameInstance()->Set_PerspectiveProjMatrix(0U, matPersProj);

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

CDynamicCamera* CDynamicCamera::Create()
{
    ThisClass* pInstance = new ThisClass();

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

    if (FAILED(pInstance->Initialize(Arg)))
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
