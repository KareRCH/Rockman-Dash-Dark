#include "System/Data/MeshData.h"

FMeshData* FMeshData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (nullptr == pInstance)
	{
		MSG_BOX("FMeshData Create Failed");
	}

	return pInstance;
}

void FMeshData::Free()
{
	vecVertices.clear();
	vecIndices.clear();
}


//----------AnimGroup---------------------------------------------------------------------------------------------


FMeshGroup* FMeshGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FMeshGroup Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FMeshGroup::Free()
{
	for (auto& Pair : mapMeshDatas)
		Safe_Release(Pair.second);
	mapMeshDatas.clear();
}

FMeshData* const FMeshGroup::Find_Mesh(const _uint iIndex) const
{
	if (iIndex < 0 && iIndex >= vecMeshDatas.size())
		return nullptr;

	return vecMeshDatas[iIndex];
}

FMeshData* const FMeshGroup::Find_Mesh(const wstring& strMeshKey) const
{
	auto iter = mapMeshDatas.find(strMeshKey);
	if (iter == mapMeshDatas.end())
		return nullptr;

	return (*iter).second;
}

HRESULT FMeshGroup::Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData)
{
	// 맵 컨테이너
	auto iter = mapMeshDatas.find(strMeshKey);
	if (iter != mapMeshDatas.end())
		return E_FAIL;

	mapMeshDatas.emplace(strMeshKey, pMeshData);

	// 벡터 컨테이너
	vecMeshDatas.resize(pMeshData->iID + 1, nullptr);
	vecMeshDatas[pMeshData->iID] = pMeshData;

	return S_OK;
}

