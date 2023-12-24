#include "Component/BoxBufferComp.h"

CBoxBufferComp::CBoxBufferComp(const CBoxBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CBoxBufferComp::Initialize_Prototype(void* Arg)
{
	m_iNumVertices = 8;
	m_iNumIndices = 36;

	// 정적 정점 버퍼의 구조체를 설정
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SHADER_VTX_CUBETEX) * m_iNumVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	SHADER_VTX_CUBETEX* vertices = new SHADER_VTX_CUBETEX[m_iNumVertices];
	if (!vertices)
		return E_FAIL;
	
	vertices[0].vPosition = vertices[0].vTexCoord = _float3(-0.5f, 0.5f, -0.5f);
	vertices[1].vPosition = vertices[1].vTexCoord = _float3(0.5f, 0.5f, -0.5f);

	vertices[2].vPosition = vertices[2].vTexCoord = _float3(0.5f, -0.5f, -0.5f);
	vertices[3].vPosition = vertices[3].vTexCoord = _float3(-0.5f, -0.5f, -0.5f);

	vertices[4].vPosition = vertices[4].vTexCoord = _float3(-0.5f, 0.5f, 0.5f);
	vertices[5].vPosition = vertices[5].vTexCoord = _float3(0.5f, 0.5f, 0.5f);

	vertices[6].vPosition = vertices[6].vTexCoord = _float3(0.5f, -0.5f, 0.5f);
	vertices[7].vPosition = vertices[7].vTexCoord = _float3(-0.5f, -0.5f, 0.5f);

	// 정점 데이터에 대한 포인터 제공
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexData, m_pVB.GetAddressOf()), E_FAIL);






	// 정적 인덱스 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_ushort) * m_iNumIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	_ushort* indices = new _ushort[m_iNumIndices];
	if (!indices)
	{
		return E_FAIL;
	}

	indices[0] = 1; indices[1] = 5; indices[2] = 6;
	indices[3] = 1; indices[4] = 6; indices[5] = 2;

	indices[6] = 4; indices[7] = 0; indices[8] = 3;
	indices[9] = 4; indices[10] = 3; indices[11] = 7;

	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	indices[24] = 5; indices[25] = 4; indices[26] = 7;
	indices[27] = 5; indices[28] = 7; indices[29] = 6;

	indices[30] = 0; indices[31] = 1; indices[32] = 2;
	indices[33] = 0; indices[34] = 2; indices[35] = 3;

	// 정적 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&indexBufferDesc, &indexData, m_pIB.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	// 추가 세팅
	m_iVertexStride = sizeof(SHADER_VTX_CUBETEX);
	m_iNumVertexBuffers = 1;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;

	return S_OK;
}

HRESULT CBoxBufferComp::Initialize(void* Arg)
{
	return S_OK;
}

CBoxBufferComp* CBoxBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("BoxBufferComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBoxBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("BoxBufferComp Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CBoxBufferComp::Free()
{
	SUPER::Free();
}
