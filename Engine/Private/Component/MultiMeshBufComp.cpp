#include "Component/MultiMeshBufComp.h"

#include "System/GameInstance.h"
#include "System/Data/MeshData.h"

CMultiMeshBufComp::CMultiMeshBufComp(const CMultiMeshBufComp& rhs)
	: Base(rhs)
{
}

HRESULT CMultiMeshBufComp::Initialize_Prototype(void* Arg)
{
	SUPER::Initialize_Prototype();

	return S_OK;
}

HRESULT CMultiMeshBufComp::Initialize(void* Arg)
{
	SUPER::Initialize();

	return S_OK;
}

HRESULT CMultiMeshBufComp::Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey)
{
	// ������ ���ε� �Ǿ� �ִ� ���� ������ ���� �����ش�.
	if (m_strBindedModel != L"")
	{
		m_strBindedModel = L"";
		m_vecMeshes.clear();
		Safe_Release(m_pMeshGroup);
		m_pMeshGroup = nullptr;
		m_strBindedMeshs.clear();
	}

	const FMeshGroup* pMeshGroup = GI()->Get_MeshGroup(eGroup, strModelKey);

	if (!pMeshGroup)
		return E_FAIL;

	m_strBindedModel = strModelKey;
	m_pMeshGroup = ConCast<FMeshGroup*>(pMeshGroup);
	Safe_AddRef(m_pMeshGroup);
	m_vecMeshes.resize(m_pMeshGroup->mapMeshData.size(), {});

	return S_OK;
}

HRESULT CMultiMeshBufComp::Bind_Mesh(const wstring& strMeshKey)
{
	// �޽� �׷��� ���ε� �Ǿ� �ִ��� Ȯ��
	if (!m_pMeshGroup)
		return E_FAIL;

	// �ߺ� �޽� �˻�
	auto Pair = m_strBindedMeshs.emplace(strMeshKey);
	if (!Pair.second)
		return E_FAIL;

	// �޽� ������
	const FMeshData* pMesh = m_pMeshGroup->Get_Mesh(strMeshKey);
	if (!pMesh)
		return E_FAIL;

	FMeshBuffer tMeshBuffer;
	tMeshBuffer.iVtxCount = Cast<_uint>(pMesh->vecVertices.size());
	tMeshBuffer.iIndCount = Cast<_uint>(pMesh->vecIndices.size());
	tMeshBuffer.strMeshName = strMeshKey;

	
	// ����, �ε��� ���� �����
	VERTEX_MODEL_SKIN_T* vertices = new VERTEX_MODEL_SKIN_T[tMeshBuffer.iVtxCount];
	if (!vertices)
		return E_FAIL;

	_uint* indices = new _uint[tMeshBuffer.iIndCount];
	if (!indices)
		return E_FAIL;

	// ���� ���� ����
	for (_uint i = 0; i < tMeshBuffer.iVtxCount; i++)
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
	vertexBufferDesc.ByteWidth = sizeof(VERTEX_MODEL_SKIN_T) * tMeshBuffer.iVtxCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&vertexBufferDesc, &vertexData, tMeshBuffer.pVtxBuffer.GetAddressOf()), E_FAIL);


	// �ε��� ���� ����
	for (_uint i = 0; i < tMeshBuffer.iIndCount; i++)
	{
		indices[i] = pMesh->vecIndices[i];
	}

	// ���� �ε��� ������ ����ü�� �����Ѵ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(_uint) * tMeshBuffer.iIndCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� �ε��� �����͸� ����Ű�� ���� ���ҽ� ����ü�� �ۼ�
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&indexBufferDesc, &indexData, tMeshBuffer.pIndBuffer.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);



	// �� ��������� �޽� ���� ������ ���Ϳ� �־��ش�.
	m_vecMeshes.push_back(tMeshBuffer);

	return S_OK;
}

HRESULT CMultiMeshBufComp::Bind_MeshAll()
{
	if (!m_pMeshGroup)
		return E_FAIL;


	return S_OK;
}

HRESULT CMultiMeshBufComp::Unbind_Mesh(const wstring& strMeshKey)
{
	auto iter = m_strBindedMeshs.find(strMeshKey);
	if (iter == m_strBindedMeshs.end())
		return E_FAIL;

	_uint iIndex = (*iter).second;

	//m_vecMeshes.

	return S_OK;
}

void CMultiMeshBufComp::Unbind_AllMeshes()
{

}

void CMultiMeshBufComp::Render_Buffer(_uint iBufferIndex)
{
	_uint iStride = sizeof(VERTEX_MODEL_SKIN_T);
	_uint iOffset = 0;

	// ���� ���� Ȱ��
	D3D11Context()->IASetVertexBuffers(0, 1, m_vecMeshes[iBufferIndex].pVtxBuffer.GetAddressOf(), &iStride, &iOffset);

	// ���ؽ� ���� Ȱ��
	D3D11Context()->IASetIndexBuffer(m_vecMeshes[iBufferIndex].pIndBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���� ���� �ﰢ�� ����Ʈ
	D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

CMultiMeshBufComp* CMultiMeshBufComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("MultiMeshBufComp Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CMultiMeshBufComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("MultiMeshBufComp Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CMultiMeshBufComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pMeshGroup);
}
