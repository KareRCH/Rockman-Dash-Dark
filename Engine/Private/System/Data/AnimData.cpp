#include "System/Data/AnimData.h"

FAnimNodeData* FAnimNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FAnimNodeData::Free()
{
	vecPositions.clear();
	vecRotations.clear();
	vecScales.clear();
}

FAnimData* FAnimData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FAnimData::Free()
{
	for (auto& Pair : mapNodeAnim)
		Safe_Release(Pair.second);
	mapNodeAnim.clear();
}

const FAnimNodeData* const FAnimData::Get_AnimNodeData(const wstring& strNodeKey)
{
	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter == mapNodeAnim.end())
		return nullptr;

	return (*iter).second;
}

void FAnimData::Add_AnimNodeData(const wstring& strNodeKey, FAnimNodeData* pAnimNodeData)
{
	auto iter = mapNodeAnim.find(strNodeKey);
	if (iter != mapNodeAnim.end())
	{
		Safe_Release(pAnimNodeData);
		return;
	}

	mapNodeAnim.emplace(strNodeKey, pAnimNodeData);
}


//---------------------------------- AnimGroup
FAnimGroup* FAnimGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FAnimGroup Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FAnimGroup::Free()
{
	for (auto& Pair : mapAnimData)
		Safe_Release(Pair.second);

	mapAnimData.clear();
}

const FAnimData* const FAnimGroup::Get_AnimData(const wstring& strAnimKey)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter == mapAnimData.end())
		return nullptr;

	return (*iter).second;
}

void FAnimGroup::Add_AnimData(const wstring& strAnimKey, FAnimData* pAnimData)
{
	auto iter = mapAnimData.find(strAnimKey);
	if (iter != mapAnimData.end())
	{
		Safe_Release(pAnimData);
		return;
	}

	mapAnimData.emplace(strAnimKey, pAnimData);
}


