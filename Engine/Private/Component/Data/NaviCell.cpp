#include "Component/Data/NaviCell.h"

#include "System/GameInstance.h"
#include "Component/EffectComponent.h"
#include "Component/NaviCellBufferComp.h"

CNaviCell::CNaviCell()
{
}

CNaviCell::CNaviCell(const CNaviCell& rhs)
{
}

HRESULT CNaviCell::Initialize(const _float3* pPoints, _uint iIndex)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3));

    m_iIndex = iIndex;

    _vector		vLine = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
    XMStoreFloat3(&m_vNormals[LINE_AB], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

    vLine = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
    XMStoreFloat3(&m_vNormals[LINE_BC], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

    vLine = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);
    XMStoreFloat3(&m_vNormals[LINE_CA], XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f));

#ifdef _DEBUG
    //m_pVIBuffer = CNaviCellBufferComp::Create(pPoints);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif

    return S_OK;
}



#ifdef _DEBUG
HRESULT CNaviCell::Render(CEffectComponent* pEffectComp)
{

    /*m_pVIBuffer->Bind_VIBuffers();

    m_pVIBuffer->Render();*/



    //_float4x4		WorldMatrix;

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
#endif // _DEBUG


CNaviCell* CNaviCell::Create(const _float3* pPoints, _uint iIndex)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(pPoints, iIndex)))
    {
        MSG_BOX("BoxBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNaviCell::Free()
{
#ifdef _DEBUG	
    Safe_Release(m_pVIBuffer);
#endif	
}

_bool CNaviCell::Compare_Points(const _float3* pSourPoint, const _float3* pDestPoint)
{
    if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pSourPoint)))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
            return true;
    }
    if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pSourPoint)))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;
    }
    if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pSourPoint)))
    {
        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;

        if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
            return true;
    }

    return false;
}

_bool CNaviCell::IsIn(_fvector vPosition, _fmatrix WorldMatrix, _int* pNeighborIndex)
{
    for (size_t i = 0; i < LINE_END; i++)
    {
        _vector	vStartPoint = XMVector3TransformCoord(XMLoadFloat3(&m_vPoints[i]), WorldMatrix);
        _vector	vNormal = XMVector3TransformNormal(XMLoadFloat3(&m_vNormals[i]), WorldMatrix);

        _vector	vSourDir = vPosition - vStartPoint;

        if (0 < XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSourDir),
            XMVector3Normalize(vNormal))))
        {
            *pNeighborIndex = m_iNeighbors[i];
            return false;
        }
    }

    return true;
}

void CNaviCell::Update(_fmatrix WorldMatrix)
{
}
