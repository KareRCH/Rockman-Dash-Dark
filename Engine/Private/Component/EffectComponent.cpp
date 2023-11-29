#include "Component/EffectComponent.h"

#include "System/GameInstance.h"

CEffectComponent::CEffectComponent(const CEffectComponent& rhs)
    : Base(rhs)
    , m_pDeviceComp(rhs.m_pDeviceComp)
    , m_pEffect(rhs.m_pEffect)
    , m_InputLayouts(rhs.m_InputLayouts)
    , m_TechniqueDesc(rhs.m_TechniqueDesc)
{
}

HRESULT CEffectComponent::Initialize_Prototype(void* Arg)
{
    FEffectCompDesc tDesc = {};

    if (Arg) { tDesc = (*ReCast<FEffectCompDesc*>(Arg)); }
    else { return E_FAIL; }

    m_pDeviceComp = CD3D11DeviceComp::Create();
    m_pEffect = GI()->Find_Effect(tDesc.strEffectName);

	return S_OK;
}

HRESULT CEffectComponent::Initialize(void* Arg)
{
	return S_OK;
}

CEffectComponent* CEffectComponent::Create(FEffectCompDesc tDesc)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(VPCast(&tDesc))))
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

}

HRESULT CEffectComponent::Begin(_uint iPassIndex)
{
    if (iPassIndex >= m_TechniqueDesc.Passes)
        return E_FAIL;

    ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
    if (nullptr == pTechnique)
        return E_FAIL;

    ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(iPassIndex);

    pPass->Apply(0, D3D11Context());

    D3D11Context()->IASetInputLayout(m_InputLayouts[iPassIndex].Get());

    return S_OK;
}

HRESULT CEffectComponent::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    /* �� ���̴��� ����Ǿ��ִ� ���������� �ڵ��� ���´�.*/
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CEffectComponent::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray((_float*)pMatrix, 0, iNumMatrices);
}

HRESULT CEffectComponent::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
    if (nullptr == pSRVariable)
        return E_FAIL;

    return pSRVariable->SetResource(pSRV);
}

HRESULT CEffectComponent::Bind_SRVs(const _char* pConstantName, ID3D11ShaderResourceView** ppSRV, _uint iNumTextures)
{
    return E_NOTIMPL;
}
