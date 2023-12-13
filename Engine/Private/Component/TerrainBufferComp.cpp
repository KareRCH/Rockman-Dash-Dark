#include "Component/TerrainBufferComp.h"

#include "System/GameInstance.h"
#include "DirectXTex.h"

CTerrainBufferComp::CTerrainBufferComp(const CTerrainBufferComp& rhs)
{
}

HRESULT CTerrainBufferComp::Initialize_Prototype(void* Arg)
{
    if (FAILED(__super::Initialize_Prototype()))
        return S_OK;

    return S_OK;
}

HRESULT CTerrainBufferComp::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return S_OK;

    return S_OK;
}

CTerrainBufferComp* CTerrainBufferComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CTerrainBufferComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CTerrainBufferComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CTerrainBufferComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CTerrainBufferComp::Free()
{
    SUPER::Free();
}

HRESULT CTerrainBufferComp::Create_Buffer(const FTerrainBufInit_HeightMap tInit)
{
    if (nullptr != m_pVB || nullptr != m_pIB)
    {
        m_pVB.Reset();
        m_pIB.Reset();
    }

    wstring strMainPath = GI()->Get_TextureMainPath();
    TexMetadata tMetaData = {};
    ScratchImage imageScratch = {};
    if (FAILED(LoadFromWICFile((strMainPath + tInit.strHeightMapFilePath).c_str(), WIC_FLAGS_NONE, &tMetaData, imageScratch)))
        return E_FAIL;

    /*_ulong dwByte = 0;
    HANDLE		hFile = CreateFile((strMainPath + tInit.strHeightMapFilePath).c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (0 == hFile)
        return E_FAIL;

    BITMAPFILEHEADER		fh;
    if (!ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr))
        return E_FAIL;

    BITMAPINFOHEADER		ih;
    if (!ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr))
        return E_FAIL;

    _uint* pPixel = new _uint[Cast<_uint>(ih.biWidth * ih.biHeight)];
    if (!ReadFile(hFile, pPixel, sizeof(_uint) * ih.biWidth * ih.biHeight, &dwByte, nullptr))
        return E_FAIL;

    CloseHandle(hFile);*/

    const DirectX::Image* img = imageScratch.GetImages();
    const size_t imageSize = tMetaData.width * tMetaData.height * sizeof(_float);
    _float* pPixel = new _float[Cast<_uint>(tMetaData.width * tMetaData.height)];
    memcpy(pPixel, img->pixels, imageSize);
    
    // ����Ʈ �� ������
    m_viNumTerrainVertices.x = Cast<_uint>(tMetaData.width);
    m_viNumTerrainVertices.z = Cast<_uint>(tMetaData.height);

    // ������ �Ÿ�
    if (m_viNumTerrainVertices.x >= m_viNumTerrainVertices.z)
        m_fInterval = Cast<_float>(tInit.iMaxWidth) / Cast<_float>(m_viNumTerrainVertices.x);
    else
        m_fInterval = Cast<_float>(tInit.iMaxWidth) / Cast<_float>(m_viNumTerrainVertices.z);

    // ���� ����
    m_iNumVertexBuffers = 1;
    m_iNumVertices = m_viNumTerrainVertices.x * m_viNumTerrainVertices.z;
    m_iVertexStride = sizeof(SHADER_VTX_NORM);

    // �ε��� ����
    m_iNumIndices = (m_viNumTerrainVertices.x - 1) * (m_viNumTerrainVertices.z - 1) * 2 * 3;
    //m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
    //m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;



#pragma region ���ؽ� ���� ����
    SHADER_VTX_NORM* pVertices = new SHADER_VTX_NORM[m_iNumVertices];

    for (_uint i = 0; i < Cast<_uint>(m_viNumTerrainVertices.z); i++)
    {
        for (_uint j = 0; j < Cast<_uint>(m_viNumTerrainVertices.x); j++)
        {
            _uint		iIndex = (i * m_viNumTerrainVertices.x) + j;
            _float      fCalcHeight = Cast<_float>(pPixel[iIndex]);

            pVertices[iIndex].vPosition = _float3(j * m_fInterval, fCalcHeight, i * m_fInterval);
            pVertices[iIndex].vNormal = _float3(0.0f, 0.0f, 0.f);
            pVertices[iIndex].vTexcoord = _float2(j / (m_viNumTerrainVertices.x - 1.0f), i / (m_viNumTerrainVertices.z - 1.0f));
        }
    }

    Safe_Delete_Array(pPixel);
#pragma endregion

#pragma region INDEX_BUFFER
    ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

    _uint* pIndices = new _uint[m_iNumIndices];

    _uint		iNumIndices = 0;

    for (_uint i = 0; i < Cast<_uint>(m_viNumTerrainVertices.z - 1); i++)
    {
        for (_uint j = 0; j < Cast<_uint>(m_viNumTerrainVertices.x - 1); j++)
        {
            _uint		iIndex = i * m_viNumTerrainVertices.x + j;

            _uint		iIndices[4] = {
                iIndex + m_viNumTerrainVertices.x,
                iIndex + m_viNumTerrainVertices.x + 1,
                iIndex + 1,
                iIndex
            };

            _vector		vSourDir, vDestDir, vNormal;

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[1];
            pIndices[iNumIndices++] = iIndices[2];

            vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
            vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
            vNormal = XMVector3Cross(vSourDir, vDestDir);

            vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[2];
            pIndices[iNumIndices++] = iIndices[3];

            vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
            vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
            vNormal = XMVector3Cross(vSourDir, vDestDir);

            vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMVector3Normalize(vNormal));
        }
    }
#pragma endregion



    m_BufferDesc = {};
    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;

    m_SubResourceData = {};
    m_SubResourceData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(m_pVB.GetAddressOf())))
        return E_FAIL;

    m_BufferDesc = {};
    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = 0;

    m_SubResourceData = {};
    m_SubResourceData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(m_pIB.GetAddressOf())))
        return E_FAIL;

    Safe_Delete_Array(pIndices);
    Safe_Delete_Array(pVertices);

    return S_OK;
}

HRESULT CTerrainBufferComp::Create_Buffer(const FTerrainBufInit_NoHeightMap tInit)
{
    if (nullptr != m_pVB || nullptr != m_pIB)
    {
        m_pVB.Reset();
        m_pIB.Reset();
    }

    // ����Ʈ �� ������
    m_viNumTerrainVertices.x = tInit.iNumVertexX;
    m_viNumTerrainVertices.z = tInit.iNumVertexZ;

    // ������ �Ÿ�
    if (m_viNumTerrainVertices.x > m_viNumTerrainVertices.z)
        m_fInterval = Cast<_float>(tInit.iMaxWidth) / Cast<_float>(m_viNumTerrainVertices.x);
    else
        m_fInterval = Cast<_float>(tInit.iMaxWidth) / Cast<_float>(m_viNumTerrainVertices.z);

    // ���� ����
    m_iNumVertexBuffers = 1;
    m_iNumVertices = m_viNumTerrainVertices.x * m_viNumTerrainVertices.z;
    m_iVertexStride = sizeof(SHADER_VTX_NORM);

    // �ε��� ����
    m_iNumIndices = (m_viNumTerrainVertices.x - 1) * (m_viNumTerrainVertices.z - 1) * 2 * 3;
    //m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
    //m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_iIndexStride = 4;
    m_eIndexFormat = DXGI_FORMAT_R32_UINT;
    m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;



#pragma region ���ؽ� ���� ����
    SHADER_VTX_NORM* pVertices = new SHADER_VTX_NORM[m_iNumVertices];

    for (_uint i = 0; i < Cast<_uint>(m_viNumTerrainVertices.z); i++)
    {
        for (_uint j = 0; j < Cast<_uint>(m_viNumTerrainVertices.x); j++)
        {
            _uint		iIndex = (i * m_viNumTerrainVertices.x) + j;

            pVertices[iIndex].vPosition = _float3(j * m_fInterval, 0.f, i * m_fInterval);
            pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
            pVertices[iIndex].vTexcoord = _float2(j / (m_viNumTerrainVertices.x - 1.0f), i / (m_viNumTerrainVertices.z - 1.0f));
        }
    }
#pragma endregion

#pragma region INDEX_BUFFER
    ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

    // �ε��� ���۰� 2����Ʈ �϶� �ȸ����� ����. _uint Ÿ���̾
    _uint* pIndices = new _uint[m_iNumIndices];

    _uint		iNumIndices = 0;

    for (_uint i = 0; i < Cast<_uint>(m_viNumTerrainVertices.z - 1); i++)
    {
        for (_uint j = 0; j < Cast<_uint>(m_viNumTerrainVertices.x - 1); j++)
        {
            _uint		iIndex = i * m_viNumTerrainVertices.x + j;

            _uint		iIndices[4] = {
                iIndex + m_viNumTerrainVertices.x,
                iIndex + m_viNumTerrainVertices.x + 1,
                iIndex + 1,
                iIndex
            };

            _vector		vSourDir, vDestDir, vNormal;

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[1];
            pIndices[iNumIndices++] = iIndices[2];

            vSourDir = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
            vDestDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
            vNormal = XMVector3Cross(vSourDir, vDestDir);

            vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

            pIndices[iNumIndices++] = iIndices[0];
            pIndices[iNumIndices++] = iIndices[2];
            pIndices[iNumIndices++] = iIndices[3];

            vSourDir = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
            vDestDir = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
            vNormal = XMVector3Cross(vSourDir, vDestDir);

            vNormal = XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMVector3Normalize(vNormal));

            vNormal = XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal;
            XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMVector3Normalize(vNormal));
        }
    }
#pragma endregion



    m_BufferDesc = {};
    m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iVertexStride;

    m_SubResourceData = {};
    m_SubResourceData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(m_pVB.GetAddressOf())))
        return E_FAIL;

    m_BufferDesc = {};
    m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = 0;

    m_SubResourceData = {};
    m_SubResourceData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(m_pIB.GetAddressOf())))
        return E_FAIL;

    Safe_Delete_Array(pIndices);
    Safe_Delete_Array(pVertices);

    return S_OK;
}

HRESULT CTerrainBufferComp::Bind_Buffer()
{
    if (nullptr == m_pVB || nullptr == m_pIB)
        return E_FAIL;

    ID3D11Buffer* pVBs[] = {
        m_pVB.Get()
    };

    _uint           iStrides[] = {
        m_iVertexStride
    };
    _uint           iOffsets[] = {
        0,
    };

    /* � ���ؽ� ���۵��� �̿��ҰŴ�. */
    D3D11Context()->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBs, iStrides, iOffsets);

    /* � �ε��� ���۸� �̿��ҰŴ�. */
    D3D11Context()->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);

    /* ������ ������� �̾ �׸��Ŵ�. */
    D3D11Context()->IASetPrimitiveTopology(m_eTopology);

    return S_OK;
}

HRESULT CTerrainBufferComp::Render_Buffer()
{
    D3D11Context()->DrawIndexed(m_iNumIndices, 0, 0);

    return S_OK;
}

HRESULT CTerrainBufferComp::IsRender_Ready()
{
    if (m_pIB == nullptr || m_pVB == nullptr)
        return E_FAIL;

    return S_OK;
}

