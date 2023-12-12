#include "Component/NaviCellBufferComp.h"

CNaviCellBufferComp::CNaviCellBufferComp(const CNaviCellBufferComp& rhs)
    : Base(rhs)
{
}

HRESULT CNaviCellBufferComp::Initialize_Prototype(const _float3* pPoints)
{
	// 디바이스 컴포넌트 참조
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


	m_iNumVertexBuffers = 1;
	m_iNumVertices = 3;
	m_iStride = sizeof(SHADER_VTX_SINGLE);

	m_iNumIndices = 4;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	SHADER_VTX_SINGLE* pVertices = new SHADER_VTX_SINGLE[m_iNumVertices];
	pVertices[0].vPosition = pPoints[0];
	pVertices[1].vPosition = pPoints[1];
	pVertices[2].vPosition = pPoints[2];

	m_SubResourceData.pSysMem = pVertices;

	/* pVertices에 할당하여 채워놨던 정점들의 정보를 ID3D11Buffer로 할당한 공간에 복사하여 채워넣는다. */
	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	_ushort* pIndices = new _ushort[m_iNumIndices];

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;
	pIndices[3] = 0;

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

    return S_OK;
}

HRESULT CNaviCellBufferComp::Initialize(void* Arg)
{
	// 암것도 없지만 넣음 
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

    return S_OK;
}

CNaviCellBufferComp* CNaviCellBufferComp::Create(const _float3* pPoints)
{
	ThisClass* pInstance = new ThisClass();

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize_Prototype(pPoints)))
	{
		MSG_BOX("Failed to Created : CNaviCellBufferComp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CNaviCellBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	/* 원형객체를 초기화한다.  */
	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("Failed to Cloned : CNaviCellBufferComp");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNaviCellBufferComp::Free()
{
	SUPER::Free();
}
