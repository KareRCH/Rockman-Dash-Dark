#include "BaseClass/Terrain.h"

CTerrain::CTerrain()
{
    Set_Name(TEXT("Terrain"));
    Set_RenderGroup(ERenderGroup::NonBlend);
}

CTerrain::CTerrain(const CTerrain& rhs)
    : Base(rhs)
{
    // 기본 복제
    //m_pTerrainModelComp = Cast<CTerrainModelComp*>(rhs.m_pTerrainModelComp->Clone());
}

HRESULT CTerrain::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    
    FAILED_CHECK_RETURN(Add_Component(L"TerrainModelComp", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);

    return S_OK;
}

HRESULT CTerrain::Initialize_Prototype(const FInitTerrain& tInit)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(tInit), E_FAIL);

    return S_OK;
}

HRESULT CTerrain::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData ProtoData;
        InputData.Get_ObjectFromArray("Components", i, ProtoData);

        _uint iComponentID = 0;
        if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strName = "";
        if (FAILED(ProtoData.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::TerrainModel):
            NULL_CHECK_RETURN(m_pTerrainModelComp = CTerrainModelComp::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pTerrainModelComp)))
                return E_FAIL;
            m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);
            break;
        }
    }

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

HRESULT CTerrain::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;
    if (FAILED(Initialize_Component(InputData)))
        return E_FAIL;

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

CTerrain* CTerrain::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
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
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CTerrain::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("Terrain Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CTerrain::Free()
{
    SUPER::Free();
}

FSerialData CTerrain::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);
    
    return Data;
}

FSerialData CTerrain::SerializeData()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

HRESULT CTerrain::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"TerrainModelComp", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);

    return S_OK;
}

HRESULT CTerrain::Initialize_Component(FSerialData& InputData)
{
    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData InputProto;
        InputData.Get_ObjectFromArray("Components", i, InputProto);

        _uint iComponentID = 0;
        if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strProtoName = "";
        if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
            return E_FAIL;

        string strName = "";
        if (FAILED(InputProto.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::TerrainModel):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pTerrainModelComp = DynCast<CTerrainModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            break;
        }
    }

    return S_OK;
}

HRESULT CTerrain::Initialize_Component(const FInitTerrain& tInit)
{
    FAILED_CHECK_RETURN(Add_Component(L"TerrainModelComp", m_pTerrainModelComp = CTerrainModelComp::Create()), E_FAIL);
    // 셰이더 매크로 정의를 확인함.
    m_pTerrainModelComp->Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements, tInit.pShaderMacro);
    
    // 헤이트맵 없이 초기화
    if (tInit.strHeightMapPath.empty())
    {
        FTerrainBufInit_NoHeightMap tBufInit = {};
        tBufInit.iNumVertexX = tInit.iNumVertexCountX;
        tBufInit.iNumVertexZ = tInit.iNumVertexCountZ;
        tBufInit.iMaxWidth = tInit.iMaxWidth;
        m_pTerrainModelComp->Create_Buffer(tBufInit);
    }
    // 헤이트맵 가지고 초기화
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
