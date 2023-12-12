#include "Component/BoxBufferComp.h"

CBoxBufferComp::CBoxBufferComp(const CBoxBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CBoxBufferComp::Initialize_Prototype(void* Arg)
{
	m_iNumVertices = 4;
	m_iNumIndices = 6;

	SHADER_VTX_TEXCOORD* vertices = new SHADER_VTX_TEXCOORD[m_iNumVertices];
	if (!vertices)
	{
		return E_FAIL;
	}

	_ushort* indices = new _ushort[m_iNumIndices];
	if (!indices)
	{
		return E_FAIL;
	}

	vertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	vertices[0].vTexCoord = _float2(0.f, 0.f);

	vertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	vertices[1].vTexCoord = _float2(1.f, 0.f);

	vertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	vertices[2].vTexCoord = _float2(1.f, 1.f);

	vertices[2].vPosition = _float3(-0.5f, -0.5f, 0.f);
	vertices[2].vTexCoord = _float2(1.f, 0.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SHADER_VTX_TEXCOORD) * m_iNumVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// ���� �����Ϳ� ���� ������ ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexData, m_pVB.GetAddressOf()), E_FAIL);

	// ���� �ε��� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_ushort) * m_iNumIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&indexBufferDesc, &indexData, m_pIB.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

HRESULT CBoxBufferComp::Initialize(void* Arg)
{
	return S_OK;
}

void CBoxBufferComp::Render_Buffer()
{
	_uint iStride = sizeof(SHADER_VTX_TEXCOORD);
	_uint iOffset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	D3D11Context()->IASetVertexBuffers(0, 1, m_pVB.GetAddressOf(), &iStride, &iOffset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ ����
	D3D11Context()->IASetIndexBuffer(m_pIB.Get(), DXGI_FORMAT_R16_UINT, 0);

	// ���� ���۷� �׸� �⺻�� ����. �ﰢ�� ����
	D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

CBoxBufferComp* CBoxBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("BoxBufferComp Create Failed");

		return nullptr;
	}

	return pInstance;
}

CComponent* CBoxBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("BoxBufferComp Copy Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CBoxBufferComp::Free()
{
	SUPER::Free();
}
