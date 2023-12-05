#include "Component/TerrainBufferComp.h"

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

HRESULT CTerrainBufferComp::Create_Buffer(const FTerrainBufInit tInit)
{
    _ulong dwByte = 0;
    HANDLE		hFile = CreateFile(tInit.strHeightMapFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if (0 == hFile)
        return E_FAIL;

    BITMAPFILEHEADER		fh;
    if (!ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr))
        return E_FAIL;

    BITMAPINFOHEADER		ih;
    if (!ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr))
        return E_FAIL;

    _ulong* pPixel = new _ulong[ih.biWidth * ih.biHeight];
    if (!ReadFile(hFile, pPixel, sizeof(_ulong) * ih.biWidth * ih.biHeight, &dwByte, nullptr))
        return E_FAIL;

    CloseHandle(hFile);

    m_viNumTerrainVertices.x = ih.biWidth;
    m_viNumTerrainVertices.z = ih.biHeight;

    m_iNumVertexBuffers = 1;
    m_iNumVertices = m_viNumTerrainVertices.x * m_viNumTerrainVertices.z;
    m_iStride = sizeof(VERTEX_NORM_T);

    m_iNumIndices = (m_viNumTerrainVertices.x - 1) * (m_viNumTerrainVertices.z - 1) * 2 * 3;
    m_iIndexStride = m_iNumVertices >= 65535 ? 4 : 2;
    m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;



#pragma region 버텍스 버퍼 생성
    VERTEX_NORM_T* pVertices = new VERTEX_NORM_T[m_iNumVertices];

    for (size_t i = 0; i < m_viNumTerrainVertices.z; i++)
    {
        for (size_t j = 0; j < m_viNumTerrainVertices.x; j++)
        {
            _uint		iIndex = (i * m_viNumTerrainVertices.x) + j;

            pVertices[iIndex].vPosition = _float3(j, (pPixel[iIndex] & 0x000000ff) / Cast<_float>(tInit.iHeight), i);
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

    for (_uint i = 0; i < m_viNumTerrainVertices.z - 1; i++)
    {
        for (_uint j = 0; j < m_viNumTerrainVertices.x - 1; j++)
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
    m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_BufferDesc.StructureByteStride = m_iStride;

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
        return;

    ID3D11Buffer* pVBs[] = {
        m_pVB.Get()
    };

    _uint           iStrides[] = {
        m_iStride
    };
    _uint           iOffsets[] = {
        0,
    };

    /* 어떤 버텍스 버퍼들을 이용할거다. */
    D3D11Context()->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBs, iStrides, iOffsets);

    /* 어떤 인덱스 버퍼를 이용할거다. */
    D3D11Context()->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);

    /* 정점을 어떤식으로 이어서 그릴거다. */
    D3D11Context()->IASetPrimitiveTopology(m_eTopology);
}

HRESULT CTerrainBufferComp::Render_Buffer()
{
    D3D11Context()->DrawIndexed(m_iNumIndices, 0, 0);

    return S_OK;
}
