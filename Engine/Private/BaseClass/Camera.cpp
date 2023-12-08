#include "BaseClass/Camera.h"

CCamera::CCamera()
    : m_eCamType(ECamType::Pers), m_eCamID(ECamNum::One)
    , m_vAt(_float3(0.f, 0.f, 0.f))
    , m_fFov(60.f), m_fAspect(Cast<_float>(g_iWindowSizeX) / Cast<_float>(g_iWindowSizeY))
    , m_fNear(0.1f), m_fFar(1000.f)
    , m_matView(), m_matProj()
{
    XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
    XMStoreFloat4x4(&m_matProj, XMMatrixIdentity());
}

CCamera::CCamera(const CCamera& rhs)
    : Base(rhs)
    , m_matView(rhs.m_matView)
    , m_matProj(rhs.m_matProj)
{
}

HRESULT CCamera::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    return S_OK;
}

HRESULT CCamera::Initialize(void* Arg)
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

void CCamera::Apply_ViewProjMatrix()
{
    _matrix matPersProj = XMMatrixPerspectiveFovLH(XMConvertToRadians(m_fFov), m_fAspect, m_fNear, m_fFar);

    m_matView = Transform().Get_TransformInverseFloat4x4();
    XMStoreFloat4x4(&m_matProj, matPersProj);



    PipelineComp().Set_ViewMatrix(m_eCamType, m_eCamID, m_matView);
    PipelineComp().Set_ProjMatrix(m_eCamType, m_eCamID, matPersProj);
}
