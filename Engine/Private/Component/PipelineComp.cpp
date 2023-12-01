#include "Component/PipelineComp.h"

#include "System/GameInstance.h"
#include "System/PipelineMgr.h"

CPipelineComp::CPipelineComp(const CPipelineComp& rhs)
    : Base(rhs)
    , m_pPipelineMgr(rhs.m_pPipelineMgr)
{
    Safe_AddRef(m_pPipelineMgr);
}

HRESULT CPipelineComp::Initialize_Prototype(void* Arg)
{
    m_pPipelineMgr = GI()->Get_PipelineMgr();
    Safe_AddRef(m_pPipelineMgr);

	return S_OK;
}

HRESULT CPipelineComp::Initialize(void* Arg)
{
	return S_OK;
}

CPipelineComp* CPipelineComp::Create()
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

CComponent* CPipelineComp::Clone(void* Arg)
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

void CPipelineComp::Free()
{
    Safe_Release(m_pPipelineMgr);
}

const _float4x4 CPipelineComp::Get_ViewFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4x4();

    return m_pPipelineMgr->Get_ViewFloat4x4(eType, eNum);
}

const _matrix CPipelineComp::Get_ViewMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _matrix();

    return m_pPipelineMgr->Get_ViewMatrix(eType, eNum);
}

const _float4x4 CPipelineComp::Get_ProjFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4x4();

    return m_pPipelineMgr->Get_ProjFloat4x4(eType, eNum);
}

const _matrix CPipelineComp::Get_ProjMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _matrix();

    return m_pPipelineMgr->Get_ProjMatrix(eType, eNum);
}

const _float4x4 CPipelineComp::Get_ProjInvFloat4x4(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4x4();

    return m_pPipelineMgr->Get_ProjInvFloat4x4(eType, eNum);
}

const _matrix CPipelineComp::Get_ProjInvMatrix(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _matrix();

    return m_pPipelineMgr->Get_ProjInvMatrix(eType, eNum);
}

void CPipelineComp::Set_ViewMatrix(ECamType eType, ECamNum eNum, _float4x4 mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_ViewMatrix(eType, eNum, mat);
}

void CPipelineComp::Set_ViewMatrix(ECamType eType, ECamNum eNum, _matrix mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_ViewMatrix(eType, eNum, mat);
}

void CPipelineComp::Set_ProjMatrix(ECamType eType, ECamNum eNum, _float4x4 mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_ProjMatrix(eType, eNum, mat);
}

void CPipelineComp::Set_ProjMatrix(ECamType eType, ECamNum eNum, _matrix mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_ProjMatrix(eType, eNum, mat);
}

const D3D11_VIEWPORT CPipelineComp::Get_Viewport(EViewportNum eNum)
{
    if (!m_pPipelineMgr)
        return D3D11_VIEWPORT();

    return m_pPipelineMgr->Get_Viewport(eNum);
}
