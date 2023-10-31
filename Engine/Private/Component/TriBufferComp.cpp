#include "Component/TriBufferComp.h"

CTriBufferComp::CTriBufferComp(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
{
}

CTriBufferComp::CTriBufferComp(const CTriBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CTriBufferComp::Initialize()
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
	vertices[0].vColor = _float4(0.f, 1.f, 0.f, 1.f);

	vertices[1].vPosition = _float3(0.f, 1.f, 0.f);
	vertices[1].vColor = _float4(0.f, 1.f, 0.f, 1.f);

	vertices[2].vPosition = _float3(1.f, -1.f, 0.f);
	vertices[2].vColor = _float4(0.f, 1.f, 0.f, 1.f);

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

	if (FAILED(m_pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &m_pVtxBuffer)))
	{
		return E_FAIL;
	}


	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

_int CTriBufferComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CTriBufferComp::LateTick()
{
}

void CTriBufferComp::Render(ID3D11DeviceContext* pDeviceContext)
{
	_uint iStride = sizeof(VTXCOL);
	_uint iOffset = 0;

	// ������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ ����
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVtxBuffer, &iStride, &iOffset);

	// ������ �� �� �ֵ��� �Է� ��������� �ε��� ���۸� Ȱ������ ����
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// ���� ���۷� �׸� �⺻�� ����. �ﰢ�� ����
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

CTriBufferComp* CTriBufferComp::Create(ID3D11Device* pGraphicDev)
{
	ThisClass* pInstance = new ThisClass(pGraphicDev);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TriBufferComp Create Failed");

		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CTriBufferComp::Clone()
{
	return new ThisClass(*this);
}

void CTriBufferComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pVtxBuffer);
	Safe_Release(m_pIndexBuffer);
}
