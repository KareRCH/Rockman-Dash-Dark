#include "Component/TerrainModelComp.h"

CTerrainModelComp::CTerrainModelComp(const CTerrainModelComp& rhs)
    : Base(rhs)
    , m_pEffectComp(rhs.m_pEffectComp)
    , m_pTerrainBufferComp(rhs.m_pTerrainBufferComp)
{
    Safe_AddRef(m_pEffectComp);
    Safe_AddRef(m_pTerrainBufferComp);
}

HRESULT CTerrainModelComp::Initialize_Prototype(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    m_pTerrainBufferComp = CTerrainBufferComp::Create();
    m_pEffectComp = CEffectComponent::Create();

	return S_OK;
}

HRESULT CTerrainModelComp::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	return S_OK;
}

void CTerrainModelComp::Priority_Tick(const _float& fTimeDelta)
{
}

void CTerrainModelComp::Tick(const _float& fTimeDelta)
{

}

void CTerrainModelComp::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CTerrainModelComp::Render()
{
    Bind_ShaderResources();

    m_pEffectComp->Begin(0);

    m_pTerrainBufferComp->Bind_Buffer();

    m_pTerrainBufferComp->Render_Buffer();

    return S_OK;
}

CTerrainModelComp* CTerrainModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CTerrainModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CTerrainModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CTerrainModelComp::Free()
{
    SUPER::Free();

    Safe_Release(m_pEffectComp);
    Safe_Release(m_pTerrainBufferComp);
}

HRESULT CTerrainModelComp::IsRender_Ready()
{
    if (!m_pTerrainBufferComp || !m_pEffectComp)
        return E_FAIL;

    if (FAILED(m_pTerrainBufferComp->IsRender_Ready()))
        return E_FAIL;

    if (FAILED(m_pEffectComp->IsRender_Ready()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrainModelComp::Bind_ShaderResources()
{
    _float4x4 matTemp = {};

    if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &Transform().Get_TransformFloat4x4())))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_ViewFloat4x4(ECamType::Pers, ECamNum::One)))))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_ProjFloat4x4(ECamType::Pers, ECamNum::One)))))
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrainModelComp::Create_Buffer(const FTerrainBufInit tInit)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Create_Buffer(tInit);
}

HRESULT CTerrainModelComp::Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Bind_Effect(strEffectKey, pElements, iNumElements);
}

HRESULT CTerrainModelComp::Unbind_Effect()
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Unbind_Effect();
}
