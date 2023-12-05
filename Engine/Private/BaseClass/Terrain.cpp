#include "BaseClass/Terrain.h"

CTerrain::CTerrain(const CTerrain& rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::Initialize(void* Arg)
{
    return S_OK;
}

void CTerrain::Priority_Tick(const _float& fTimeDelta)
{
}

_int CTerrain::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTerrain::Late_Tick(const _float& fTimeDelta)
{
}

void CTerrain::Render()
{
    m_pTerrainModelComp->Render();
}

CTerrain* CTerrain::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Terrain Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CGameObject* CTerrain::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Terrain Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    /* Arg에서 넘겨받은 데이터 적용하기 */

    return Cast<CGameObject*>(pInstance);
}

void CTerrain::Free()
{
    SUPER::Free();
}

HRESULT CTerrain::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);

    m_pTerrainModelComp->Create_Buffer({L"HeightMap", 100, 100});
    m_pTerrainModelComp->Bind_Effect(L"FX_Terrain");
    

    return S_OK;
}
