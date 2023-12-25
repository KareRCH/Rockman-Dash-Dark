#include "System/Data/MeshData.h"

#include "System/GameInstance.h"
#include "ModelLoad.h"

HRESULT FMeshData::Initialize(const TMesh& MeshData)
{
	iID = MeshData.iID;
	iMaterialIndex = MeshData.iMaterialIndex;

	// 버퍼 생성
	if (FAILED(Create_Buffer(MeshData)))
		return E_FAIL;


	// 뼈 오프셋 정보
	vecMeshBoneDatas.resize(MeshData.vecBones.size());
	for (_uint i = 0; i < MeshData.vecBones.size(); i++)
	{
		const TMeshBone& tMeshBone = MeshData.vecBones[i];

		vecMeshBoneDatas[i].iBoneID = tMeshBone.iBoneID;
		vecMeshBoneDatas[i].matOffset = tMeshBone.matOffset;
	}

	return S_OK;
}

HRESULT FMeshData::Create_Buffer(const TMesh& MeshData)
{
	iNumVertices = Cast<_uint>(MeshData.vecVertices.size());
	iVertexStride = Cast<_uint>(sizeof(SHADER_VTX_SKINMODEL));

	iNumIndices = Cast<_uint>(MeshData.vecIndices.size());
	iIndexStride = (65536 > iNumIndices) ? 2 : 4;
	eIndexFormat = (iIndexStride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// 정점, 인덱스 버퍼 만들기
	SHADER_VTX_SKINMODEL* vertices = new SHADER_VTX_SKINMODEL[iNumVertices];
	if (!vertices)
		return E_FAIL;

	// 정점 버퍼 제작
	for (_uint i = 0; i < iNumVertices; i++)
	{
		vertices[i].vPosition = MeshData.vecVertices[i].vPosition;
		vertices[i].vNormal = MeshData.vecVertices[i].vNormal;
		vertices[i].vTexCoord = MeshData.vecVertices[i].vTexCoord;
		vertices[i].vTangent = MeshData.vecVertices[i].vTangent;

		memcpy_s(&vertices[i].vBoneID, sizeof(_int4), MeshData.vecVertices[i].vecBoneID.data(), sizeof(_int4));
		memcpy_s(&vertices[i].vWeight, sizeof(_float4), MeshData.vecVertices[i].vecWeights.data(), sizeof(_float4));
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SHADER_VTX_SKINMODEL) * iNumVertices;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;
	
	FAILED_CHECK_RETURN(GI()->Get_GraphicDev()->CreateBuffer(&vertexBufferDesc, &vertexData, pVB.GetAddressOf()), E_FAIL);





	// 인덱스 버퍼 제작
	void* indices = nullptr;
	
	if (iIndexStride == 2)
		indices = new _ushort[iNumIndices];
	else
		indices = new _uint[iNumIndices];
		
	if (!indices)
		return E_FAIL;

	memcpy(indices, MeshData.vecIndices.data(), iIndexStride * iNumIndices);

	// 정적 인덱스 버퍼의 구조체를 설정한다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = iIndexStride * iNumIndices;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 정적 인덱스 데이터를 가리키는 보조 리소스 구조체를 작성
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	FAILED_CHECK_RETURN(GI()->Get_GraphicDev()->CreateBuffer(&indexBufferDesc, &indexData, pIB.GetAddressOf()), E_FAIL);

	Safe_Delete_Array(vertices);
	Safe_Delete_Array(indices);

	return S_OK;
}

FMeshData* FMeshData::Create(const TMesh& MeshData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(MeshData)))
	{
		MSG_BOX("FMeshData Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void FMeshData::Free()
{

}


//----------AnimGroup---------------------------------------------------------------------------------------------


CMeshGroup* CMeshGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("CMeshGroup Create Failed");
	}

	return pInstance;
}

void CMeshGroup::Free()
{
	for (auto& Pair : mapMeshDatas)
		Safe_Release(Pair.second);
	mapMeshDatas.clear();
}

FMeshData* const CMeshGroup::Find_Mesh(const _uint iIndex) const
{
	if (iIndex < 0 && iIndex >= vecMeshDatas.size())
		return nullptr;

	return vecMeshDatas[iIndex];
}

FMeshData* const CMeshGroup::Find_Mesh(const wstring& strMeshKey, const _uint iRangeIndex) const
{
	auto range = mapMeshDatas.equal_range(strMeshKey);

	_uint i = 0;
	auto iter = range.first;
	for (; iter != range.second; ++iter)
	{
		if (iRangeIndex == i++)
			break;
	}
	if (iter == range.second)
		return nullptr;


	return (*iter).second;
}

HRESULT CMeshGroup::Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData)
{
	if (pMeshData == nullptr)
		return E_FAIL;

	// 멀티맵 컨테이너, 메쉬는 중복이 일어날 수 있음
	mapMeshDatas.emplace(strMeshKey, pMeshData);

	// 벡터 컨테이너
	vecMeshDatas.resize(Cast<size_t>(pMeshData->iID + 1), nullptr);
	vecMeshDatas[pMeshData->iID] = pMeshData;

	++m_iNumMeshes;

	return S_OK;
}

