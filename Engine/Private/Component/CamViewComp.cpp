#include "Component/CamViewComp.h"

#include "System/GameInstance.h"
#include "System/CamViewMgr.h"

CCamViewComp::CCamViewComp(const CCamViewComp& rhs)
    : Base(rhs)
    , m_pCamViewMgr(rhs.m_pCamViewMgr)
{
}

HRESULT CCamViewComp::Initialize_Prototype(void* Arg)
{
    m_pCamViewMgr = GI()->Get_CamViewMgr();
    Safe_AddRef(m_pCamViewMgr);

	return S_OK;
}

HRESULT CCamViewComp::Initialize(void* Arg)
{
	return S_OK;
}

CCamViewComp* CCamViewComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CamViewComp Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CCamViewComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CamViewComp Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CCamViewComp::Free()
{
    Safe_Release(m_pCamViewMgr);
}

const _float4x4 CCamViewComp::Get_ViewFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _float4x4();

    return m_pCamViewMgr->Get_ViewFloat4x4(eType, eNum);
}

const _matrix CCamViewComp::Get_ViewMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _matrix();

    return m_pCamViewMgr->Get_ViewMatrix(eType, eNum);
}

const _float4x4 CCamViewComp::Get_ProjFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _float4x4();

    return m_pCamViewMgr->Get_ProjFloat4x4(eType, eNum);
}

const _matrix CCamViewComp::Get_ProjMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _matrix();

    return m_pCamViewMgr->Get_ProjMatrix(eType, eNum);
}

const _float4x4 CCamViewComp::Get_ProjInvFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _float4x4();

    return m_pCamViewMgr->Get_ProjInvFloat4x4(eType, eNum);
}

const _matrix CCamViewComp::Get_ProjInvMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pCamViewMgr)
        return _matrix();

    return m_pCamViewMgr->Get_ProjInvMatrix(eType, eNum);
}

const D3D11_VIEWPORT CCamViewComp::Get_Viewport(EViewportNum eNum)
{
    if (!m_pCamViewMgr)
        return D3D11_VIEWPORT();

    return m_pCamViewMgr->Get_Viewport(eNum);
}
