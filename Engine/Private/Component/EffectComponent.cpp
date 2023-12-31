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
    // 디바이스 참조 연결, 내부 자동 레퍼카운트 증가
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
    // 셰이더 매니저에 의존성이 있음, 항상 전역적으로 다룸
    if (FAILED(Link_ShaderMgr()))
        return E_FAIL;

    // 있던거 끊기
    Unbind_Effect();

    // 있던거면 파일 이름으로 검색해서 찾는다.
    FEffectData* pEffectData = m_pShaderMgr->Find_EffectData(strEffectFileName);
    if (!pEffectData)
    {
        // 없으면 로드 시키고, 그래도 실패하면 파일 못 찾는거임.
        if (FAILED(m_pShaderMgr->Load_Effect(strEffectFileName, pElements, iNumElements, pShaderMacro)))
            return E_FAIL;

        // 로드성공시 다시 찾아온다.
        pEffectData = m_pShaderMgr->Find_EffectData(strEffectFileName);
    }

    m_pEffectData = pEffectData;
    Safe_AddRef(m_pEffectData);

    return S_OK;
}

HRESULT CEffectComponent::Unbind_Effect()
{
    // 릴리즈시 이 포인터는 널로 바꾼다. 재사용 가능성이 있을 때 사용한다.
    Safe_ReleaseAndUnlink(m_pEffectData);

    return S_OK;
}

HRESULT CEffectComponent::IsRender_Ready()
{
    // 이펙트랑 연결되지 않았음.
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

    /* 이 셰이더에 선언되어있는 전역변수의 핸들을 얻어온다.*/
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
