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
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

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

    Transform().Set_Position(_float3(3.f, 3.f, -6.f));

    _float3 vPos, vUp, vAt;
    vPos = Transform().Get_PositionFloat3();
    vAt = _float3(0.f, 0.f, 0.f);
    vUp = _float3(0.f, 1.f, 0.f);

    _matrix matPersView, matPersProj;

    matPersView = XMMatrixLookAtLH(XMLoadFloat3(&vPos), XMLoadFloat3(&vAt), XMLoadFloat3(&vUp));
    matPersProj = XMMatrixPerspectiveFovLH(XM_PI * 0.25f, ((_float)g_iWindowSizeX / (_float)g_iWindowSizeY), 0.1f, 1000.f);
    //m_matPersProj = XMMatrixOrthographicLH(g_iWindowSizeX, g_iWindowSizeY, 0.1f, 1000.f);

    XMStoreFloat4x4(&m_matPersView, matPersView);
    XMStoreFloat4x4(&m_matPersProj, matPersProj);

    PipelineComp().Set_ViewMatrix(ECamType::Pers, ECamNum::One, matPersView);
    PipelineComp().Set_ProjMatrix(ECamType::Pers, ECamNum::One, matPersProj);

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

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CGameObject* CDynamicCamera::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    /* Arg Ã³¸® */

    return Cast<CGameObject*>(pInstance);
}

void CDynamicCamera::Free()
{
    SUPER::Free();
}
