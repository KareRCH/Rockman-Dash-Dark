#include "System/PipelineMgr.h"

HRESULT CPipelineMgr::Initialize()
{
    _float4x4 matIdentity;
    XMStoreFloat4x4(&matIdentity, XMMatrixIdentity());
    for (_uint i = 0; i < ECast(ECamType::Size); i++)
    {
        for (_uint j = 0; j < ECast(ECamMatrix::Size); j++)
        {
            for (_uint k = 0; k < ECast(ECamNum::Size); k++)
            {
                m_bActiveCams[i][k] = false;
                m_CamMatrices[i][j][k] = matIdentity;
                m_CamInvMatrices[i][j][k] = matIdentity;
            }
        }
    }

    for (_uint i = 0; i < ECast(EViewportNum::Size); i++)
    {
        m_Viewports[i].TopLeftX = 0.f;
        m_Viewports[i].TopLeftY = 0.f;
        m_Viewports[i].Width = g_iWindowSizeX;
        m_Viewports[i].Height = g_iWindowSizeY;
        m_Viewports[i].MinDepth = 0.f;
        m_Viewports[i].MaxDepth = 1.f;
    }

    Active_Camera(ECamType::Persp, ECamNum::One);

    return S_OK;
}

void CPipelineMgr::Tick()
{
    for (_uint i = 0; i < ECast(ECamType::Size); i++)
    {
        for (_uint j = 0; j < ECast(ECamMatrix::Size); j++)
        {
            for (_uint k = 0; k < ECast(ECamNum::Size); k++)
            {
                // 안 쓰는 카메라 연산 안함.
                if (!m_bActiveCams[i][k])
                    continue;

                _matrix matInv = XMLoadFloat4x4(&m_CamMatrices[i][j][k]);
                _float4x4 matResult;
                _vector vDeterminant = XMMatrixDeterminant(matInv);
                matInv = XMMatrixInverse(&vDeterminant, matInv);
                XMStoreFloat4x4(&matResult, matInv);

                m_CamInvMatrices[i][j][k] = matResult;
                // 위치는 View일 때만 저장한다.
                if (j == 0)
                    memcpy(&m_CamPositions[i][k], m_CamInvMatrices[i][j][k].m[3], sizeof(_float4));
            }
        }
    }
}

CPipelineMgr* CPipelineMgr::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ProtoMgr Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPipelineMgr::Free()
{
}
