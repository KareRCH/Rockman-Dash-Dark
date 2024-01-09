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

    return S_OK;
}

HRESULT CTerrain::Initialize_Prototype(const FInitTerrain& tInit)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(tInit), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // ������ ����, Tick�� �۵���, ���Ǥ�
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer ���

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
    }

    return pInstance;
}

CTerrain* CTerrain::Create(const FInitTerrain& tInit)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(tInit)))
    {
        MSG_BOX("Terrain Create Failed");
        Safe_Release(pInstance);
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

FSerialData CTerrain::SerializeData()
{
    return FSerialData();
}

HRESULT CTerrain::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"TerrainModelComp", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);

    return S_OK;
}

HRESULT CTerrain::Initialize_Component(const FInitTerrain& tInit)
{
    FAILED_CHECK_RETURN(Add_Component(L"TerrainModelComp", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    // ���̴� ��ũ�� ���Ǹ� Ȯ����.
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements, tInit.pShaderMacro);
    
    // ����Ʈ�� ���� �ʱ�ȭ
    if (tInit.strHeightMapPath.empty())
    {
        FTerrainBufInit_NoHeightMap tBufInit = {};
        tBufInit.iNumVertexX = tInit.iNumVertexCountX;
        tBufInit.iNumVertexZ = tInit.iNumVertexCountZ;
        tBufInit.iMaxWidth = tInit.iMaxWidth;
        m_pTerrainModelComp->Create_Buffer(tBufInit);
    }
    // ����Ʈ�� ������ �ʱ�ȭ
    else
    {
        FTerrainBufInit_HeightMap tBufInit = {};
        tBufInit.strHeightMapFilePath = tInit.strHeightMapPath;
        tBufInit.iMaxWidth = tInit.iMaxWidth;
        m_pTerrainModelComp->Create_Buffer(tBufInit);
    }

    return S_OK;
}

HRESULT CTerrain::Create_Terrain(const FInitTerrain& tInit)
{
    if (nullptr == m_pTerrainModelComp)
        return E_FAIL;

    FTerrainBufInit_NoHeightMap tBufInit = {};
    tBufInit.iNumVertexX = tInit.iNumVertexCountX;
    tBufInit.iNumVertexZ = tInit.iNumVertexCountZ;
    tBufInit.iMaxWidth = tInit.iMaxWidth;
    

    m_pTerrainModelComp->Create_Buffer(tBufInit);

    return S_OK;
}

HRESULT CTerrain::Create_TerrainByHeightMap(const FInitTerrain& tInit)
{
    if (nullptr == m_pTerrainModelComp)
        return E_FAIL;

    FTerrainBufInit_HeightMap tBufInit = {};
    tBufInit.strHeightMapFilePath = tInit.strHeightMapPath;
    tBufInit.iMaxWidth = tInit.iMaxWidth;

    m_pTerrainModelComp->Create_Buffer(tBufInit);

    return S_OK;
}
