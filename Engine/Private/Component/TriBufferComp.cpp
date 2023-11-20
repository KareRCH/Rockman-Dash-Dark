#include "Component/TriBufferComp.h"

CTriBufferComp::CTriBufferComp(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

CTriBufferComp::CTriBufferComp(const CTriBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CTriBufferComp::Initialize(void* Arg)
{
	m_iVtxCount = 3;
	m_iIndexCount = 3;

	VTXCOL* vertices = new VTXCOL[m_iVtxCount];
	if (!vertices)
	{
		return E_FAIL;
	}

	_uint* indices = new _uint[m_iIndexCount];
	if (!indices)
	{
		return E_FAIL;
	}

	vertices[0].vPosition = _float3(-1.f, -1.f, 0.f);
	vertices[0].vColor = _float4(1.f, 0.f, 0.f, 1.f);

	vertices[1].vPosition = _float3(1.f, 1.f, 0.f);
	vertices[1].vColor = _float4(0.f, 1.f, 0.f, 1.f);

	vertices[2].vPosition = _float3(1.f, -1.f, 0.f);
	vertices[2].vColor = _float4(0.f, 0.f, 1.f, 1.f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	// ���� ���� ������ ����ü�� ����
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VTXCOL) * m_iVtxCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// ���� �����Ϳ� ���� ������ ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVtxBuffer), E_FAIL);

	// ���� �ε��� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_uint) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer), E_FAIL);


	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

void CTriBufferComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CTriBufferComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CTriBufferComp::Late_Tick(const _float& fTimeDelta)
{
}

void CTriBufferComp::Render()
{
	_uint iStride = sizeof(VTXCOL);
	_uint iOffset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &iStride, &iOffset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ ����
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻�� ����. �ﰢ�� ����
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

CTriBufferComp* CTriBufferComp::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TriBufferComp Create Failed");

		return nullptr;
	}

	return pInstance;
}

CComponent* CTriBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TriBufferComp Copy Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTriBufferComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pVtxBuffer);
	Safe_Release(m_pIndexBuffer);
}
