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

const _float4x4 CPipelineComp::Get_CamFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4x4();

    return m_pPipelineMgr->Get_CamFloat4x4(eType, eMatrix, eNum);
}

const _matrix CPipelineComp::Get_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _matrix();

    return m_pPipelineMgr->Get_CamMatrix(eType, eMatrix, eNum);
}

const _float4x4 CPipelineComp::Get_CamInvFloat4x4(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4x4();

    return m_pPipelineMgr->Get_CamInvFloat4x4(eType, eMatrix, eNum);
}

const _matrix CPipelineComp::Get_CamInvMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _matrix();

    return m_pPipelineMgr->Get_CamInvMatrix(eType, eMatrix, eNum);
}

void CPipelineComp::Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _float4x4 mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_CamMatrix(eType, eMatrix, eNum, mat);
}

void CPipelineComp::Set_CamMatrix(ECamType eType, ECamMatrix eMatrix, ECamNum eNum, _fmatrix mat)
{
    if (!m_pPipelineMgr)
        return;

    return m_pPipelineMgr->Set_CamMatrix(eType, eMatrix, eNum, mat);
}

const _float4 CPipelineComp::Get_CamPositionFloat4(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _float4();

    return m_pPipelineMgr->Get_CamPositionFloat4(eType, eNum);
}

const _vector CPipelineComp::Get_CamPositionVector(ECamType eType, ECamNum eNum) const
{
    if (!m_pPipelineMgr)
        return _vector();

    return m_pPipelineMgr->Get_CamPositionVector(eType, eNum);
}

const D3D11_VIEWPORT CPipelineComp::Get_Viewport(EViewportNum eNum)
{
    if (!m_pPipelineMgr)
        return D3D11_VIEWPORT();

    return m_pPipelineMgr->Get_Viewport(eNum);
}
