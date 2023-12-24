#include "Component/CylinderBufferComp.h"

CCylinderBufferComp::CCylinderBufferComp(const CCylinderBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CCylinderBufferComp::Initialize_Prototype(void* Arg)
{

	return S_OK;
}

HRESULT CCylinderBufferComp::Initialize(void* Arg)
{
	return S_OK;
}

CCylinderBufferComp* CCylinderBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CBoxModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCylinderBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CBoxModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CCylinderBufferComp::Free()
{
	SUPER::Free();
}

HRESULT CCylinderBufferComp::Create_Buffer(const TBufferDesc tDesc)
{
	if (tDesc.iNumSegments < 3)
		return E_FAIL;

	m_iNumVertices = tDesc.iNumSegments * 2 + 2;
	m_iNumIndices = tDesc.iNumSegments * 4 * 3;

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
	{
		return E_FAIL;
	}

	_uint iStride = m_iNumVertices / 2;
	_uint iStrideWithoutCenter = iStride - 1;
	for (_uint i = 0; i < 2; i++)
	{
		_float fY = (i == 0) ? -0.5f : 0.5f;

		for (_uint j = 0; j < iStrideWithoutCenter; j++)
		{
			_uint iIndex = iStride * i + j;
			_float fX, fZ;

			_float fDegree = 90.f + (360.f / iStrideWithoutCenter) * j;
			fX = cosf(XMConvertToRadians(fDegree)) * 0.5f;
			fZ = sinf(XMConvertToRadians(fDegree)) * 0.5f;

			vertices[iIndex].vPosition = vertices[iIndex].vTexCoord = _float3(fX, fY, fZ);

			if (IsOnRange(fDegree - 45.f, 0.f, 90.f))
			{
				vertices[iIndex].vTexCoord.z = 0.5f;
			}
			else if (IsOnRange(fDegree - 45.f, 90.f, 180.f))
			{
				vertices[iIndex].vTexCoord.x = -0.5f;
			}
			else if (IsOnRange(fDegree - 45.f, 180.f, 270.f))
			{
				vertices[iIndex].vTexCoord.z = -0.5f;
			}
			else if (IsOnRange(fDegree - 45.f, 270.f, 360.f))
			{
				vertices[iIndex].vTexCoord.x = 0.5f;
			}
		}

		_uint iCenterIndex = iStride * (i + 1) - 1;
		vertices[iCenterIndex].vPosition = vertices[iCenterIndex].vTexCoord = _float3(0.f, fY, 0.f);
	}

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

	_uint iIndex = 0;
	_uint iIndexPass = tDesc.iNumSegments;
	_uint iHalfVertex = tDesc.iNumSegments + 1;

	// 옆면
	for (_uint i = 0; i < iIndexPass; i++)
	{
		if (i < iIndexPass - 1)
		{
			indices[iIndex++] = i;
			indices[iIndex++] = i + 1 + iHalfVertex;
			indices[iIndex++] = i + 1;

			indices[iIndex++] = i;
			indices[iIndex++] = i + iHalfVertex;
			indices[iIndex++] = i + iHalfVertex + 1;
		}
		// 마지막꺼
		else
		{
			indices[iIndex++] = i;
			indices[iIndex++] = iHalfVertex;
			indices[iIndex++] = 0;

			indices[iIndex++] = i;
			indices[iIndex++] = i + iHalfVertex;
			indices[iIndex++] = iHalfVertex;
		}
	}

	// 상, 하단
	for (_uint i = 0; i < iIndexPass; i++)
	{
		if (i < iIndexPass - 1)
		{
			indices[iIndex++] = i;
			indices[iIndex++] = i + 1;
			indices[iIndex++] = iHalfVertex - 1;

			indices[iIndex++] = (iHalfVertex * 2 - 1);
			indices[iIndex++] = i + iHalfVertex + 1;
			indices[iIndex++] = i + iHalfVertex;
		}
		else
		{
			indices[iIndex++] = i;
			indices[iIndex++] = 0;
			indices[iIndex++] = iHalfVertex - 1;

			indices[iIndex++] = (iHalfVertex * 2 - 1);
			indices[iIndex++] = iHalfVertex;
			indices[iIndex++] = i + iHalfVertex;
		}
	}

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
