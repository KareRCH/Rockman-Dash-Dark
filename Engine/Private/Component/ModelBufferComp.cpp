#include "Component/ModelBufferComp.h"

#include "System/GameInstance.h"
#include "System/ModelMgr.h"

CModelBufferComp::CModelBufferComp(const CModelBufferComp& rhs)
    : Base(rhs)
{
}

HRESULT CModelBufferComp::Initialize_Prototype(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

	return S_OK;
}

HRESULT CModelBufferComp::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

    return S_OK;
}

HRESULT CModelBufferComp::Initialize(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strModelKey)
{
	const FMeshData* pMesh = GameInstance()->Find_MeshData(eGroupIndex, strGroupKey, strModelKey);

	m_iNumVertices = Cast<_uint>(pMesh->vecVertices.size());
	m_iNumIndices = Cast<_uint>(pMesh->vecIndices.size());

	VERTEX_MODEL_SKIN_T* vertices = new VERTEX_MODEL_SKIN_T[m_iNumVertices];
	if (!vertices)
		return E_FAIL;

	_uint* indices = new _uint[m_iNumIndices];
	if (!indices)
		return E_FAIL;

	// 정점 버퍼 제작
	for (_uint i = 0; i < m_iNumVertices; i++)
	{
		vertices[i].vPosition = pMesh->vecVertices[i].vPosition;
		vertices[i].vNormal = pMesh->vecVertices[i].vNormal;
		vertices[i].vTexCoord = pMesh->vecVertices[i].vTexCoord;
		vertices[i].vTangent = pMesh->vecVertices[i].vTangent;
		for (_uint j = 0; j < (_uint)pMesh->vecVertices[j].vecBoneID.size() / 4; j++)
		{
			_uint iIndex = j * 4;
			vertices[i].vBoneID.x = pMesh->vecVertices[i].vecBoneID[iIndex];
			vertices[i].vBoneID.y = pMesh->vecVertices[i].vecBoneID[iIndex + 1];
			vertices[i].vBoneID.z = pMesh->vecVertices[i].vecBoneID[iIndex + 2];
			vertices[i].vBoneID.w = pMesh->vecVertices[i].vecBoneID[iIndex + 3];

			vertices[i].vWeight.x = pMesh->vecVertices[i].vecWeights[iIndex];
			vertices[i].vWeight.y = pMesh->vecVertices[i].vecWeights[iIndex + 1];
			vertices[i].vWeight.z = pMesh->vecVertices[i].vecWeights[iIndex + 2];
			vertices[i].vWeight.w = pMesh->vecVertices[i].vecWeights[iIndex + 3];
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_MODEL_SKIN_T) * m_iNumVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexData, m_pVtxBuffer.GetAddressOf()), E_FAIL);

	// 인덱스 버퍼 제작
	for (_uint i = 0; i < m_iNumIndices; i++)
	{
		indices[i] = pMesh->vecIndices[i];
	}

	// 정적 인덱스 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_uint) * m_iNumIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 정적 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&indexBufferDesc, &indexData, m_pIndexBuffer.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

void CModelBufferComp::Render_Buffer()
{
	if (m_pVtxBuffer == nullptr || m_pIndexBuffer == nullptr)
		return;

	_uint iStride = sizeof(VERTEX_MODEL_SKIN_T);
	_uint iOffset = 0;

	// 정점 버퍼 활성
	D3D11Context()->IASetVertexBuffers(0, 1, m_pVtxBuffer.GetAddressOf(), &iStride, &iOffset);

	// 인텍스 버퍼 활성
	D3D11Context()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 정점 버퍼 삼각형 리스트
	D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

CModelBufferComp* CModelBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("ModelBufferComp Create Failed");
		Safe_Release(pInstance);
		
		return nullptr;
	}

	return pInstance;
}

CComponent* CModelBufferComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("ModelBufferComp Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CModelBufferComp::Free()
{
    SUPER::Free();
}


