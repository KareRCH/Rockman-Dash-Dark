#include "System/Data/MaterialData.h"

FMaterialData* FMaterialData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (nullptr == pInstance)
	{
		MSG_BOX("FMeshData Create Failed");
	}

	return pInstance;
}

void FMaterialData::Free()
{
}

FMaterialGroup* FMaterialGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (nullptr == pInstance)
	{
		MSG_BOX("FMaterialGroup Create Failed");
	}

	return pInstance;
}

void FMaterialGroup::Free()
{
}

FMaterialData* FMaterialGroup::Find_Material(const _uint iIndex)
{
	if (iIndex < 0 && iIndex >= vecMaterialDatas.size())
		return nullptr;

	return vecMaterialDatas[iIndex];
}

FMaterialData* FMaterialGroup::Find_Material(const wstring& strName)
{
	auto iter = mapMaterialDatas.find(strName);
	if (iter == mapMaterialDatas.end())
		return nullptr;

	return (*iter).second;
}
