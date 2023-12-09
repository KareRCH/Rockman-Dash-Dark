#include "GameObject/ToolCamera.h"

CToolCamera::CToolCamera()
{
    Set_Name(L"ToolCamera");
}

CToolCamera::CToolCamera(const CToolCamera& rhs)
    : Base(rhs)
{
}

HRESULT CToolCamera::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    Transform().Set_Position(_float3(0.f, 0.f, -6.f));
    _matrix matTransform = XMMatrixInverse(nullptr,
        XMMatrixLookAtLH(Transform().Get_PositionVector(), XMLoadFloat3(&m_vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
    Transform().Set_Transform(matTransform);
    Apply_ViewProjMatrix();

    return S_OK;
}

HRESULT CToolCamera::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

void CToolCamera::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

_int CToolCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_W))
        Transform().MoveForward(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_S))
        Transform().MoveForward(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_D))
        Transform().MoveRightward(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_A))
        Transform().MoveRightward(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_E))
        Transform().MoveUpward(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_Q))
        Transform().MoveUpward(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressed(DIK_F3))
        GI()->Toggle_LockMouseCenter();

    //Transform().TurnAxis(_float3(0.f, 1.f, 0.f), Cast<_float>(GI()->Get_DIMouseMove(DIMS_X)) * 0.1f * fTimeDelta);
    //Transform().TurnUp(Cast<_float>(GI()->Get_DIMouseMove(DIMS_Y)) * 0.1f * fTimeDelta);

    // 현재 카메라의 상태를 통해 전역 카메라 행렬을 업데이트 한다.
    Apply_ViewProjMatrix();

    return _int();
}

void CToolCamera::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

void CToolCamera::Render()
{

}

CToolCamera* CToolCamera::Create()
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

CGameObject* CToolCamera::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    /* Arg 처리 */

    return Cast<CGameObject*>(pInstance);
}

void CToolCamera::Free()
{
    SUPER::Free();
}
