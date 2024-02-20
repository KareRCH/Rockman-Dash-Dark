#include "Component/TerrainModelComp.h"

CTerrainModelComp::CTerrainModelComp()
{
    NULL_CHECK(m_pTerrainBufferComp = CTerrainBufferComp::Create());
    NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
    for (_uint i = 0; i < TYPE_END; i++)
    {
        NULL_CHECK(m_pTextureComps[i] = CTextureComponent::Create());
    }
}

CTerrainModelComp::CTerrainModelComp(const CTerrainModelComp& rhs)
    : Base(rhs)
    , m_pEffectComp(rhs.m_pEffectComp)
    , m_pTerrainBufferComp(rhs.m_pTerrainBufferComp)
{
    Safe_AddRef(m_pEffectComp);
    Safe_AddRef(m_pTerrainBufferComp);
    for (_uint i = 0; i < TYPE_END; i++)
    {
        NULL_CHECK(m_pTextureComps[i] = rhs.m_pTextureComps[i]);
        Safe_AddRef(m_pTextureComps[i]);
    }
}

HRESULT CTerrainModelComp::Initialize_Prototype(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CTerrainModelComp::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    _uint iMaxWidth = 0;
    InputData.Get_Data("MaxWidth", iMaxWidth);

    string strHeightMapPath = "";
    InputData.Get_Data("HeightMapPath", strHeightMapPath);

    FTerrainBufInit_HeightMap tInit = {};
    tInit.iMaxWidth = iMaxWidth;
    tInit.strHeightMapFilePath = ConvertToWstring(strHeightMapPath);
    if (FAILED(m_pTerrainBufferComp->Create_Buffer(tInit)))
        return E_FAIL;

    string strDiffuseTexture;
    InputData.Get_Data("DiffuseTexture1", strDiffuseTexture);
    m_strDiffuseTexture = ConvertToWstring(strDiffuseTexture);

    string strMaskTexture;
    InputData.Get_Data("MaskTexture1", strMaskTexture);
    m_strMaskTexture = ConvertToWstring(strMaskTexture);

    if (FAILED(Bind_Texture(CTerrainModelComp::TYPE_DIFFUSE, m_strDiffuseTexture, 1)))
        return E_FAIL;
    Bind_Texture(CTerrainModelComp::TYPE_MASK, m_strMaskTexture, 1);

    Bind_Effect(TEXT("Runtime/FX_Terrain.hlsl"), SHADER_VTX_NORM::Elements, SHADER_VTX_NORM::iNumElements);

    return S_OK;
}

HRESULT CTerrainModelComp::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	return S_OK;
}

HRESULT CTerrainModelComp::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

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

    for (_uint i = 0; i < m_iNumActivePasses; i++)
    {
        m_pEffectComp->Begin(m_vecActivePasses[i]);

        m_pTerrainBufferComp->Bind_Buffer();

        m_pTerrainBufferComp->Render_Buffer();
    }

    return S_OK;
}

FSerialData CTerrainModelComp::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", g_ClassID);

    Data.Add_Member("MaxWidth", m_pTerrainBufferComp->Get_MaxWidth());
    Data.Add_MemberString("HeightMapPath", ConvertToString(m_pTerrainBufferComp->Get_HeightMapPath()));
    Data.Add_MemberString("DiffuseTexture1", ConvertToString(m_strDiffuseTexture));
    Data.Add_MemberString("MaskTexture1", ConvertToString(m_strMaskTexture));

    return Data;
}

FSerialData CTerrainModelComp::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

CTerrainModelComp* CTerrainModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CTerrainModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CTerrainModelComp* CTerrainModelComp::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CTerrainModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTerrainModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TerrainModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

CComponent* CTerrainModelComp::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("TerrainModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CTerrainModelComp::Free()
{
    SUPER::Free();

    Safe_Release(m_pEffectComp);
    Safe_Release(m_pTerrainBufferComp);
    for (size_t i = 0; i < TYPE_END; i++)
        Safe_Release(m_pTextureComps[i]);
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
    _float4 vTemp = {};

    if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &(matTemp = Calculate_TransformFloat4x4FromParent()))))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
        return E_FAIL;
    if (FAILED(m_pTextureComps[TYPE_DIFFUSE]->Bind_SRVToEffect(m_pEffectComp, "g_DiffuseTexture", 0)))
        return E_FAIL;
    /*if (FAILED(m_pTextureComps[TYPE_MASK]->Bind_SRVToEffect(m_pEffectComp, "g_MaskTexture", 0)))
        return E_FAIL;*/
    /*if (FAILED(m_pTextureComps[TYPE_BRUSH]->Bind_SRVToEffect(m_pEffectComp, "g_BrushTexture", 0)))
        return E_FAIL;*/
    if (FAILED(m_pEffectComp->Bind_RawValue("g_vCamPosition", &(vTemp = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One)), sizeof(_float4))))
        return E_FAIL;
    // 클라이언트에서 하면 안될거임.
    auto pOwnerObj = Get_OwnerObject();
    if (nullptr != pOwnerObj)
    {
        m_pEffectComp->Bind_RawValue("g_iObjectID", &Get_OwnerObject()->Get_ID(), sizeof(_int));
    }

    return S_OK;
}

HRESULT CTerrainModelComp::Create_Buffer(const FTerrainBufInit_HeightMap tInit)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Create_Buffer(tInit);
}

HRESULT CTerrainModelComp::Create_Buffer(const FTerrainBufInit_NoHeightMap tInit)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Create_Buffer(tInit);
}

HRESULT CTerrainModelComp::Copy_VBuffer(void* pArray, size_t iSize, size_t iSizePerIndex)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Copy_VBufferToArray(pArray, iSize, iSizePerIndex);
}

void CTerrainModelComp::Update_VBuffer(void* pData, _uint iSize)
{
    if (!m_pTerrainBufferComp)
        return;

    m_pTerrainBufferComp->Update_VBuffer(pData, iSize);
}

HRESULT CTerrainModelComp::Copy_IBuffer(void* pArray, size_t iSize)
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Copy_IBufferToArray(pArray, iSize);
}

const size_t CTerrainModelComp::Get_VertexCount() const
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Get_VertexCount();
}

const size_t CTerrainModelComp::Get_IndexCount() const
{
    if (!m_pTerrainBufferComp)
        return E_FAIL;

    return m_pTerrainBufferComp->Get_IndexCount();
}


HRESULT CTerrainModelComp::Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO* pShaderMacro)
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Bind_Effect(strEffectKey, pElements, iNumElements, pShaderMacro);
}

HRESULT CTerrainModelComp::Unbind_Effect()
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Unbind_Effect();
}

HRESULT CTerrainModelComp::Bind_Texture(TEXTURE eType, const wstring& strFileName, const _uint iNumTextures)
{
    if (!m_pTextureComps[eType])
        return E_FAIL;

    if (FAILED(m_pTextureComps[eType]->Load_Texture(strFileName, iNumTextures)))
        return E_FAIL;

    switch (eType)
    {
    case TEXTURE::TYPE_DIFFUSE:
        m_strDiffuseTexture = strFileName;
        break;
    case TEXTURE::TYPE_MASK:
        m_strMaskTexture = strFileName;
        break;
    }

    return S_OK;
}

HRESULT CTerrainModelComp::Unbind_Texture(TEXTURE eType)
{
    if (!m_pTextureComps[eType])
        return E_FAIL;

    return m_pTextureComps[eType]->Unload_Texture();
}
