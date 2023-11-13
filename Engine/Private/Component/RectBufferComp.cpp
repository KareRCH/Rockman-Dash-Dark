#include "Component/RectBufferComp.h"

CRectBufferComp::CRectBufferComp(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

CRectBufferComp::CRectBufferComp(const CRectBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CRectBufferComp::Initialize()
{
	m_iVtxCount = 4;
	m_iIndexCount = 6;

	VERTEX_TEXTURE_T* vertices = new VERTEX_TEXTURE_T[m_iVtxCount];
	if (!vertices)
	{
		return E_FAIL;
	}

	_ushort* indices = new _ushort[m_iIndexCount];
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
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_TEXTURE_T) * m_iVtxCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;


	// ���� �����Ϳ� ���� ������ ����
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_pVtxBuffer.GetAddressOf()), E_FAIL);

	// ���� �ε��� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_ushort) * m_iIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&indexBufferDesc, &indexData, m_pIndexBuffer.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

void CRectBufferComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CRectBufferComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CRectBufferComp::Late_Tick(const _float& fTimeDelta)
{
}

void CRectBufferComp::Render()
{
	_uint iStride = sizeof(VTXCOL);
	_uint iOffset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVtxBuffer.GetAddressOf(), &iStride, &iOffset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ ����
	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// ���� ���۷� �׸� �⺻�� ����. �ﰢ�� ����
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

CRectBufferComp* CRectBufferComp::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("RectBufferComp Create Failed");

		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CRectBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("RectBufferComp Copy Failed");

		return nullptr;
	}

	return pInstance;
}

void CRectBufferComp::Free()
{
	SUPER::Free();
}
