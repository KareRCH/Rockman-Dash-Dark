#include "System/Data/BoneData.h"

FBoneData* FBoneData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FBoneData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FBoneData::Free()
{
	vecVtxIndex.clear();
	vecVtxWeights.clear();
}

//----------------------------------------- AnimGroup
FBoneGroup* FBoneGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FBoneGroup Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FBoneGroup::Free()
{
	for (auto& Pair : mapBoneData)
	{
		Safe_Release(Pair.second);
	}
	mapBoneData.clear();
}

const FBoneData* const FBoneGroup::Get_Bone(const wstring& strBoneKey)
{
	auto iter = mapBoneData.find(strBoneKey);
	if (iter == mapBoneData.end())
		return nullptr;

	return (*iter).second;
}

void FBoneGroup::Add_Bone(const wstring& strBoneKey, FBoneData* pBoneData)
{
	auto iter = mapBoneData.find(strBoneKey);
	if (iter != mapBoneData.end())
	{
		Safe_Release(pBoneData);
		return;
	}

	mapBoneData.emplace(strBoneKey, pBoneData);
}
