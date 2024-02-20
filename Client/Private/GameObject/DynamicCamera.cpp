#include "GameObject/DynamicCamera.h"

#include "GameObject/Player.h"

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

HRESULT CDynamicCamera::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    return S_OK;
}

HRESULT CDynamicCamera::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

HRESULT CDynamicCamera::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

    return S_OK;
}

void CDynamicCamera::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}


void CDynamicCamera::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    //Transform().TurnAxis(_float3(0.f, 1.f, 0.f), Cast<_float>(GI()->Get_DIMouseMove(DIMS_X)) * 0.1f * fTimeDelta);
    //Transform().TurnUp(Cast<_float>(GI()->Get_DIMouseMove(DIMS_Y)) * 0.1f * fTimeDelta);

    if (nullptr != m_pTarget)
    {
        CPlayer* pPlayer = DynCast<CPlayer*>(m_pTarget);
        if (nullptr != pPlayer && nullptr == m_pPivotComp)
        {
            m_pPivotComp = pPlayer->Get_Component<CPivotComponent>(TEXT("CameraPivot"));
        }

        if (nullptr != m_pPivotComp)
        {
            //Transform().Set_Position(m_pPivotComp->Transform().Get_PositionVector() - Transform().Get_LookNormalizedVector() * m_fSpringLength);

            m_fVerticalAngle += Cast<_float>(GI()->Get_DIMouseMove(DIMS_Y)) * 2.f * fTimeDelta;
            if (m_fVerticalAngle < -60.f)
                m_fVerticalAngle = -60.f;
            else if (m_fVerticalAngle > 60.f)
                m_fVerticalAngle = 60.f;

            _vector vLook = m_pPivotComp->Transform().Get_LookNormalizedVector();
            _vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
            _vector vRight = Transform().Get_RightNormalizedVector();

            // 거리는 각도에 따라 결정
            _float  fLength = 2.f * (1.f + ((m_fVerticalAngle + 20.f) / 60.f));

            vLook = XMVector3Rotate(vLook, XMQuaternionRotationAxis(vRight, XMConvertToRadians(m_fVerticalAngle)));
            //vLook = XMVector3Rotate(vLook, XMQuaternionRotationAxis(vUp, XMConvertToRadians(m_fVerticalAngle)));

            vLook *= fLength;
            m_fSpringLength = fLength;

            Transform().Set_Position(m_pPivotComp->Transform().Get_PositionVector() - vLook);
            XMStoreFloat3(&m_vAt, m_pPivotComp->Transform().Get_PositionVector());
            _matrix matTransform = XMMatrixInverse(nullptr,
                XMMatrixLookAtLH(Transform().Get_PositionVector(), XMLoadFloat3(&m_vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
            Transform().Set_Transform(matTransform);
        }
    }

    if (GI()->IsKey_Pressed(DIK_F3))
        GI()->Toggle_LockMouseCenter();

    // 현재 카메라의 상태를 통해 전역 카메라 행렬을 업데이트 한다.
    Apply_ViewProjMatrix();
}

void CDynamicCamera::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

HRESULT CDynamicCamera::Render()
{
    SUPER::Render();

    return S_OK;
}

void CDynamicCamera::BeginPlay()
{
    m_pTarget = m_pGI->Find_GameObjectByName(TEXT("Player"));
    if (nullptr != m_pTarget)
    {
        Safe_AddRef(m_pTarget);

        Transform().Set_Position(m_pTarget->Transform().Get_PositionVector() + XMVectorSet(0.f, 1.f, 0.f, 0.f) - m_pTarget->Transform().Get_LookVector());
        XMStoreFloat3(&m_vAt, m_pTarget->Transform().Get_PositionVector() + XMVectorSet(0.f, 1.f, 0.f, 0.f));
        _matrix matTransform = XMMatrixInverse(nullptr,
            XMMatrixLookAtLH(Transform().Get_PositionVector(), XMLoadFloat3(&m_vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
        Transform().Set_Transform(matTransform);
        Apply_ViewProjMatrix();

        m_pGI->OnOff_LockMouseCenter(true);
    }
    else
    {
        Transform().Set_Position(XMVectorSet(0.f, 5.f, 0.f, 0.f));
        XMStoreFloat3(&m_vAt, XMVectorSet(10.f, 0.f, 10.f, 0.f));
        _matrix matTransform = XMMatrixInverse(nullptr,
            XMMatrixLookAtLH(Transform().Get_PositionVector(), XMLoadFloat3(&m_vAt), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
        Transform().Set_Transform(matTransform);
        Apply_ViewProjMatrix();
    }
}

CDynamicCamera* CDynamicCamera::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CDynamicCamera* CDynamicCamera::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);
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
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CDynamicCamera::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("DynamicCamera Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CDynamicCamera::Free()
{
    SUPER::Free();

    Safe_Release(m_pTarget);
}

FSerialData CDynamicCamera::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    return Data;
}

FSerialData CDynamicCamera::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    return Data;
}
