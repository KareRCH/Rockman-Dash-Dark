#include "Component/EffectComponent.h"

#include "System/GameInstance.h"

CEffectComponent::CEffectComponent(const CEffectComponent& rhs)
    : Base(rhs)
    , m_pDeviceComp(rhs.m_pDeviceComp)
    , m_pEffectData(rhs.m_pEffectData)
    , m_pShaderMgr(rhs.m_pShaderMgr)
{
    Safe_AddRef(m_pDeviceComp);
    Safe_AddRef(m_pEffectData);
    Safe_AddRef(m_pShaderMgr);
}

HRESULT CEffectComponent::Initialize_Prototype(void* Arg)
{
    // ����̽� ���� ����, ���� �ڵ� ����ī��Ʈ ����
    m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp"));

	return S_OK;
}

HRESULT CEffectComponent::Initialize(void* Arg)
{
	return S_OK;
}

CEffectComponent* CEffectComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("EffectComponent Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CEffectComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("EffectComponent Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CEffectComponent::Free()
{
    SUPER::Free();

    Safe_Release(m_pDeviceComp);
    Safe_Release(m_pEffectData);
    Safe_Release(m_pShaderMgr);
}

HRESULT CEffectComponent::Bind_Effect(const wstring& strEffectFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO** pShaderMacro)
{
    // ���̴� �Ŵ����� �������� ����, �׻� ���������� �ٷ�
    if (FAILED(Link_ShaderMgr()))
        return E_FAIL;

    // �ִ��� ����
    Unbind_Effect();

    // �ִ��Ÿ� ���� �̸����� �˻��ؼ� ã�´�.
    FEffectData* pEffectData = m_pShaderMgr->Find_EffectData(strEffectFileName);
    if (!pEffectData)
    {
        // ������ �ε� ��Ű��, �׷��� �����ϸ� ���� �� ã�°���.
        if (FAILED(m_pShaderMgr->Load_Effect(strEffectFileName, pElements, iNumElements, pShaderMacro)))
            return E_FAIL;

        // �ε强���� �ٽ� ã�ƿ´�.
        pEffectData = m_pShaderMgr->Find_EffectData(strEffectFileName);
    }

    m_pEffectData = pEffectData;
    Safe_AddRef(m_pEffectData);

    return S_OK;
}

HRESULT CEffectComponent::Unbind_Effect()
{
    // ������� �� �����ʹ� �η� �ٲ۴�. ���� ���ɼ��� ���� �� ����Ѵ�.
    Safe_ReleaseAndUnlink(m_pEffectData);

    return S_OK;
}

HRESULT CEffectComponent::IsRender_Ready()
{
    // ����Ʈ�� ������� �ʾ���.
    if (nullptr == m_pEffectData)
        return E_FAIL;

    return S_OK;
}

HRESULT CEffectComponent::Begin(_uint iPassIndex)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    if (iPassIndex >= m_pEffectData->tTechniqueDesc.Passes)
        return E_FAIL;

    ID3DX11EffectTechnique* pTechnique = m_pEffectData->pEffect->GetTechniqueByIndex(0);
    if (nullptr == pTechnique)
        return E_FAIL;

    ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(iPassIndex);

    pPass->Apply(0, D3D11Context());

    D3D11Context()->IASetInputLayout(m_pEffectData->pInputLayouts[iPassIndex].Get());

    return S_OK;
}

HRESULT CEffectComponent::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    /* �� ���̴��� ����Ǿ��ִ� ���������� �ڵ��� ���´�.*/
    ID3DX11EffectVariable* pVariable = m_pEffectData->pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CEffectComponent::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    ID3DX11EffectVariable* pVariable = m_pEffectData->pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray((_float*)pMatrix, 0, iNumMatrices);
}

HRESULT CEffectComponent::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    ID3DX11EffectVariable* pVariable = m_pEffectData->pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResource(pSRV);
}

HRESULT CEffectComponent::Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    ID3DX11EffectVariable* pVariable = m_pEffectData->pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResourceArray(ppSRV, 0, iNumTextures);
}

HRESULT CEffectComponent::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iSize)
{
    if (nullptr == m_pEffectData || nullptr == m_pEffectData->pEffect)
        return E_FAIL;

    ID3DX11EffectVariable* pVariable = m_pEffectData->pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    return pVariable->SetRawValue(pData, 0, iSize);
}

HRESULT CEffectComponent::Link_ShaderMgr()
{
    if (m_pShaderMgr)
        return S_OK;

    if (nullptr == GI() 
        || nullptr == (m_pShaderMgr = GI()->Get_ShaderMgr()))
        return E_FAIL;

    Safe_AddRef(m_pShaderMgr);

    return S_OK;
}
