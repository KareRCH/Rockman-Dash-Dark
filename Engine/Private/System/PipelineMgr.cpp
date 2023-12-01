#include "System/PipelineMgr.h"

HRESULT CPipelineMgr::Initialize()
{
    _float4x4 matIdentity;
    XMStoreFloat4x4(&matIdentity, XMMatrixIdentity());
    for (_uint i = 0; i < ECast(ECamType::Size); i++)
    {
        for (_uint j = 0; j < ECast(ECamNum::Size); j++)
        {
            m_bActiveCams[i][j] = false;
            m_matViews[i][j] = matIdentity;
            m_matProjs[i][j] = matIdentity;
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

    Active_Camera(ECamType::Pers, ECamNum::One);
    Active_Camera(ECamType::Ortho, ECamNum::One);

    return S_OK;
}

void CPipelineMgr::Tick()
{

}

CPipelineMgr* CPipelineMgr::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ProtoMgr Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

void CPipelineMgr::Free()
{
}

const _float4x4 CPipelineMgr::Get_ProjInvFloat4x4(ECamType eType, ECamNum eNum) const
{
    _matrix matInv = XMLoadFloat4x4(&m_matProjs[ECast(eType)][ECast(eNum)]);
    _float4x4 matResult;
    _vector vDeterminant = XMMatrixDeterminant(matInv);

    matInv = XMMatrixInverse(&vDeterminant, matInv);
    XMStoreFloat4x4(&matResult, matInv);

    return matResult;
}

const _matrix CPipelineMgr::Get_ProjInvMatrix(ECamType eType, ECamNum eNum) const
{
    _matrix matInv = XMLoadFloat4x4(&m_matProjs[ECast(eType)][ECast(eNum)]);
    _vector vDeterminant = XMMatrixDeterminant(matInv);

    matInv = XMMatrixInverse(&vDeterminant, matInv);

    return matInv;
}
