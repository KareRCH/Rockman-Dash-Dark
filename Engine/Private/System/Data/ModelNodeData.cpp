#include "System/Data/ModelNodeData.h"

void FModelNodeBaseData::Free()
{
	for (auto& item : vecChildren)
	{
		Safe_Release(item);
	}
	vecChildren.clear();
}


// ------------------------ FModelNodeData ----------------------------


FModelNodeData* FModelNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FModelNodeData::Free()
{
	SUPER::Free();


}

FModelNodeData* FModelNodeData::Find_NodeFromID(_int iID)
{
	if (vecChildren.empty())
	{
		if (this->iID == iID)
			return this;
		else
			return nullptr;
	}

	_int iMin, iMax;
	iMin = vecChildren.front()->iID;
	iMax = vecChildren.back()->iID;
	
	if (iMin <= iID && iID <= iMax)
	{
		for (_uint i = 0; i < vecChildren.size(); i++)
		{
			FModelNodeData* pNode = vecChildren[i]->Find_NodeFromID(iID);
			if (pNode != nullptr)
				return pNode;
		}
	}

	return nullptr;
}

// ------------------------ ArmatureData ---------------------------
FArmatureData* FArmatureData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FArmatureData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FArmatureData::Free()
{
	for (auto& Pair : mapModelNodeData)
	{
		Safe_Release(Pair.second);
	}
	mapModelNodeData.clear();
}

FModelNodeData* FArmatureData::Find_NodeData(_int iID)
{
	if (!pArmatureNode)
		return nullptr;

	return pArmatureNode->Find_NodeFromID(iID);
}

FModelNodeData* FArmatureData::Find_NodeData(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter == mapModelNodeData.end())
		return nullptr;

	return (*iter).second;
}


FModelNodeData* FArmatureData::Create_NodeData(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter != mapModelNodeData.end())
		return (*iter).second;

	auto pNode = FModelNodeData::Create();
	mapModelNodeData.emplace(strModelNodeKey, pNode);

	return pNode;
}

void FArmatureData::Appoint_ArmatureNode(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter == mapModelNodeData.end())
		return;

	pArmatureNode = (*iter).second;
}


// ------------------------ ModelNodeGroup ----------------------------


FModelNodeGroup* FModelNodeGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

void FModelNodeGroup::Free()
{
	for (auto& Pair : mapArmatureData)
	{
		Safe_Release(Pair.second);
	}
	mapArmatureData.clear();
}

FArmatureData* FModelNodeGroup::Find_ArmatureData(const wstring& strArmatureKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second;
}

FArmatureData* FModelNodeGroup::Create_ArmatureData(const wstring& strArmatureKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter != mapArmatureData.end())
		return (*iter).second;

	auto pArmature = FArmatureData::Create();
	mapArmatureData.emplace(strArmatureKey, pArmature);

	return pArmature;
}

FModelNodeData* FModelNodeGroup::Find_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second->Find_NodeData(strModelNodeKey);
}

FModelNodeData* FModelNodeGroup::Create_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second->Create_NodeData(strModelNodeKey);
}

void FModelNodeGroup::Appoint_ArmatureNode(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return;

	(*iter).second->Appoint_ArmatureNode(strModelNodeKey);
}

