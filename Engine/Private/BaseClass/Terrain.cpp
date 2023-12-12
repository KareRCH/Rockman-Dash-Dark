#include "BaseClass/Terrain.h"

CTerrain::CTerrain(const CTerrain& rhs)
    : Base(rhs)
{
    // �⺻ ����
    m_pTerrainModelComp = Cast<CTerrainModelComp*>(rhs.m_pTerrainModelComp->Clone());
}

HRESULT CTerrain::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // ������ ����, Tick�� �۵���, ���Ǥ�
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer ���
    TurnOn_State(EGObjectState::RenderDeferred);    // ���۵� ���̵� ���, ZBuffer �̻��� ����

    return S_OK;
}

HRESULT CTerrain::Initialize(void* Arg)
{
    FInitTerrain tInit = {};

    if (nullptr == Arg)
        return E_FAIL;

    tInit = (*ReCast<FInitTerrain*>(Arg));

    FAILED_CHECK_RETURN(__super::Initialize(VPCast(&tInit)), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(tInit), E_FAIL);

    return S_OK;
}

void CTerrain::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CTerrain::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CTerrain::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CTerrain::Render()
{
    m_pTerrainModelComp->Render();

    return S_OK;
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

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("Terrain Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CGameObject*>(pInstance);
}

void CTerrain::Free()
{
    SUPER::Free();
}

HRESULT CTerrain::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::InputLayout, SHADER_VTX_NORM::iMaxIndex);

    return S_OK;
}

HRESULT CTerrain::Initialize_Component(const FInitTerrain& tInit)
{

    
    return S_OK;
}

HRESULT CTerrain::Create_Terrain(const FInitTerrain& tInit)
{
    if (nullptr == m_pTerrainModelComp)
        return E_FAIL;

    FTerrainBufInit_NoHeight tBufInit = {};
    tBufInit.iWidth = tInit.iWidthX;
    tBufInit.iHeight = tInit.iHeight;
    tBufInit.iNumVertexX = tInit.iNumVertexCountX;
    tBufInit.iNumVertexZ = tInit.iNumVertexCountZ;

    m_pTerrainModelComp->Create_Buffer(tBufInit);

    return S_OK;
}

HRESULT CTerrain::Create_TerrainByHeightMap(const FInitTerrain& tInit)
{
    if (nullptr == m_pTerrainModelComp)
        return E_FAIL;

    FTerrainBufInit tBufInit = {};
    tBufInit.strHeightMapFilePath = tInit.strHeightMapPath;
    tBufInit.iWidth = tInit.iWidthX;
    tBufInit.iHeight = tInit.iWidthX;

    m_pTerrainModelComp->Create_Buffer(tBufInit);

    return S_OK;
}
