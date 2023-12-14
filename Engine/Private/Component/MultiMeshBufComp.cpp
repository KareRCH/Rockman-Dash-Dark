#include "Component/MultiMeshBufComp.h"

#include "System/GameInstance.h"
#include "System/ModelMgr.h"

CMultiMeshBufComp::CMultiMeshBufComp(const CMultiMeshBufComp& rhs)
	: Base(rhs)
	, m_pModelData(rhs.m_pModelData)
	, m_pMeshGroup(rhs.m_pMeshGroup)
{
	Safe_AddRef(m_pModelData);
	Safe_AddRef(m_pMeshGroup);
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

	Safe_Release(m_pModelData);
	Safe_Release(m_pMeshGroup);
}


HRESULT CMultiMeshBufComp::Set_ModelData(FModelData* pModelData)
{
	// ������ ���ε� �Ǿ� �ִ� ���� ������ ���� �����ش�.
	if (!m_pModelData)
	{
		Safe_Release(m_pModelData);
		m_pModelData = nullptr;
		Safe_Release(m_pMeshGroup);
		m_pMeshGroup = nullptr;
		m_vecMeshes.clear();
		m_strBindedMeshes.clear();
	}

	FMeshGroup* pMeshGroup = pModelData->pMeshGroup;

	if (!pMeshGroup || !pModelData)
		return E_FAIL;

	m_pModelData = pModelData;
	Safe_AddRef(m_pModelData);
	m_pMeshGroup = pMeshGroup;
	Safe_AddRef(m_pMeshGroup);
	m_vecMeshes.resize(m_pMeshGroup->mapMeshDatas.size(), {});

	return S_OK;
}

HRESULT CMultiMeshBufComp::Bind_Mesh(const wstring& strMeshKey)
{
	// �޽� �׷��� ���ε� �Ǿ� �ִ��� Ȯ��
	if (!m_pMeshGroup)
		return E_FAIL;

	// �ߺ� �޽� �˻�
	auto iter = m_strBindedMeshes.find(strMeshKey);
	if (iter != m_strBindedMeshes.end())
		return E_FAIL;

	// �޽� ������
	const FMeshData* pMesh = m_pMeshGroup->Find_Mesh(strMeshKey);
	if (!pMesh)
		return E_FAIL;

	FMeshBuffer tMeshBuffer;
	tMeshBuffer.iVtxCount = Cast<_uint>(pMesh->vecVertices.size());
	tMeshBuffer.iIndCount = Cast<_uint>(pMesh->vecIndices.size());
	tMeshBuffer.strMeshName = strMeshKey;
	tMeshBuffer.matTransform = &pMesh->matOffset;

	
	// ����, �ε��� ���� �����
	SHADER_VTX_SKINMODEL* vertices = new SHADER_VTX_SKINMODEL[tMeshBuffer.iVtxCount];
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

		memcpy_s(&vertices[i].vBoneID, sizeof(_int4), pMesh->vecVertices[i].vecBoneID.data(), sizeof(_int4));
		memcpy_s(&vertices[i].vWeight, sizeof(_float4), pMesh->vecVertices[i].vecWeights.data(), sizeof(_float4));
		_int t = 0;
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SHADER_VTX_SKINMODEL) * tMeshBuffer.iVtxCount;
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
	tMeshBuffer.bBinded = true;
	m_vecMeshes[pMesh->iID] = (tMeshBuffer);

	return S_OK;
}

HRESULT CMultiMeshBufComp::Bind_MeshAll()
{
	if (!m_pMeshGroup)
		return E_FAIL;

	for (auto iter = m_pMeshGroup->mapMeshDatas.begin(); iter != m_pMeshGroup->mapMeshDatas.end(); ++iter)
	{
		Bind_Mesh((*iter).first);
	}

	return S_OK;
}

HRESULT CMultiMeshBufComp::Unbind_Mesh(const wstring& strMeshKey)
{
	auto iter = m_strBindedMeshes.find(strMeshKey);
	if (iter == m_strBindedMeshes.end())
		return E_FAIL;

	_uint iIndex = (*iter).second;

	m_vecMeshes[iIndex] = {};
	m_vecMeshes[iIndex].bBinded = false;

	return S_OK;
}

void CMultiMeshBufComp::Unbind_AllMeshes()
{

}

void CMultiMeshBufComp::Bind_Buffer(_uint iBufferIndex)
{
	_uint iStride = sizeof(SHADER_VTX_SKINMODEL);
	_uint iOffset = 0;

	// ���� ���� Ȱ��
	D3D11Context()->IASetVertexBuffers(0, 1, m_vecMeshes[iBufferIndex].pVtxBuffer.GetAddressOf(), &iStride, &iOffset);

	// ���ؽ� ���� Ȱ��
	D3D11Context()->IASetIndexBuffer(m_vecMeshes[iBufferIndex].pIndBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���� ���� �ﰢ�� ����Ʈ
	D3D11Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void CMultiMeshBufComp::Render_Buffer(_uint iBufferIndex)
{
	D3D11Context()->DrawIndexed(m_vecMeshes[iBufferIndex].iIndCount, 0, 0);
}


