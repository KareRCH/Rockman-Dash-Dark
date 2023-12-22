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

FMeshData* const FMeshGroup::Find_Mesh(const wstring& strMeshKey, const _uint iRangeIndex) const
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

HRESULT FMeshGroup::Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData)
{
	if (pMeshData == nullptr)
		return E_FAIL;

	// 멀티맵 컨테이너, 메쉬는 중복이 일어날 수 있음
	mapMeshDatas.emplace(strMeshKey, pMeshData);

	// 벡터 컨테이너
	vecMeshDatas.resize(Cast<size_t>(pMeshData->iID + 1), nullptr);
	vecMeshDatas[pMeshData->iID] = pMeshData;

	return S_OK;
}

