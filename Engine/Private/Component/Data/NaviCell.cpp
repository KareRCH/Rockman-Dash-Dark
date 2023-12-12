#include "Component/Data/NaviCell.h"

#include "System/GameInstance.h"
#include "Component/EffectComponent.h"
#include "Component/NaviCellBufferComp.h"

CNaviCell::CNaviCell(const CNaviCell& rhs)
    : m_pDeviceComp(rhs.m_pDeviceComp)
{
}

HRESULT CNaviCell::Initialize(const _float3* pPoints)
{
    // 디바이스에 접근 할 수 있는 컴포넌트 참조
    m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp"));

    rsize_t iSize = sizeof(_float3);
    memcpy_s(m_vPoints, iSize, pPoints, iSize);

#ifdef _DEBUG


    //m_pVIBuffer = CNaviCellBufferComp::Create(pPoints);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif

    return S_OK;
}

HRESULT CNaviCell::Render(CEffectComponent* pEffectComp)
{
    _float4x4		WorldMatrix;

    /*XMStoreFloat4x4(&WorldMatrix, XMMatrixIdentity());

    if (FAILED(pEffectComp->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
        return E_FAIL;
    if (FAILED(pEffectComp->Bind_Matrix("g_ViewMatrix", &PipelineComp())))
        return E_FAIL;
    if (FAILED(pEffectComp->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    pEffectComp->Begin(0);

    m_pVIBuffer->Bind_VIBuffers();

    m_pVIBuffer->Render();*/

    return S_OK;
}

CNaviCell* CNaviCell::Create(const _float3* pPoints)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(pPoints)))
    {
        MSG_BOX("BoxBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNaviCell::Free()
{
    Safe_Release(m_pDeviceComp);
}
