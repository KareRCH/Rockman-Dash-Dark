#include "Component/TerrainModelComp.h"

CTerrainModelComp::CTerrainModelComp(const CTerrainModelComp& rhs)
{
}

HRESULT CTerrainModelComp::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CTerrainModelComp::Initialize(void* Arg)
{
	return S_OK;
}

void CTerrainModelComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CTerrainModelComp::Tick(const _float& fTimeDelta)
{
	return _int();
}

void CTerrainModelComp::Late_Tick(const _float& fTimeDelta)
{
}

void CTerrainModelComp::Render()
{
    Bind_ShaderResources();

    m_pEffectComp->Begin(0);

    m_pTerrainBufferComp->Bind_Buffer();

    m_pTerrainBufferComp->Render_Buffer();
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
}

HRESULT CTerrainModelComp::Bind_ShaderResources()
{
    if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &Transform().Get_TransformFloat4x4())))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &PipelineComp().Get_ViewFloat4x4(ECamType::Pers, ECamNum::One))))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &PipelineComp().Get_ProjFloat4x4(ECamType::Pers, ECamNum::One))))
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrainModelComp::Create_Buffer(const FTerrainBufInit tInit)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Create_Buffer(tInit);
}

HRESULT CTerrainModelComp::Bind_Effect(const wstring& strEffectKey)
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Bind_Effect(strEffectKey);
}

HRESULT CTerrainModelComp::Unbind_Effect()
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Unbind_Effect();
}
