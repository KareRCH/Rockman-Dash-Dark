#include "System/Data/MeshData.h"

FMeshData* FMeshData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FMeshData Create Failed");

		return nullptr;
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
	for (auto& Pair : mapMeshData)
		Safe_Release(Pair.second);
	mapMeshData.clear();
}

const FMeshData* const FMeshGroup::Get_Mesh(const wstring& strMeshKey) const
{
	auto iter = mapMeshData.find(strMeshKey);
	if (iter == mapMeshData.end())
		return nullptr;

	return (*iter).second;
}

void FMeshGroup::Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData)
{
	auto iter = mapMeshData.find(strMeshKey);
	if (iter != mapMeshData.end())
		return;

	mapMeshData.emplace(strMeshKey, pMeshData);
}

