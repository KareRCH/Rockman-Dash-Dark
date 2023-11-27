#include "System/Data/BoneData.h"

FBoneNodeBaseData::FBoneNodeBaseData(const FBoneNodeBaseData& rhs)
	: iID(rhs.iID), eType(rhs.eType), eBoneType(rhs.eBoneType)
	, strName(rhs.strName), pParent(rhs.pParent)
	, vecChildren(rhs.vecChildren), matOffset(rhs.matOffset), matTransform(rhs.matTransform)
{
}

void FBoneNodeBaseData::Free()
{
	for (auto& item : vecChildren)
	{
		Safe_Release(item);
	}
	vecChildren.clear();
}


// ------------------------ FBoneNodeData ----------------------------

FBoneNodeData::FBoneNodeData(const FBoneNodeData& rhs)
	: Base(rhs)
{
}

FBoneNodeData* FBoneNodeData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FBoneNodeData Create Failed");

		return nullptr;
	}

	return pInstance;
}

FBoneNodeBaseData* FBoneNodeData::Clone(FArmatureData* pArg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FBoneNodeData Copy Failed");

		return nullptr;
	}

	if (!pInstance->vecChildren.empty())
	{
		for (_uint i = 0; Cast<size_t>(i) < pInstance->vecChildren.size(); i++)
		{
			FBoneNodeBaseData* pNode = pInstance->vecChildren[i]->Clone(pArg);
			pNode->pParent = pInstance;
			vecChildren[i] = Cast<FBoneNodeData*>(pNode);
			pArg->Add_NodeData(pNode->strName, Cast<FBoneNodeData*>(pNode));
		}
	}

	return pInstance;
}

void FBoneNodeData::Free()
{
	SUPER::Free();


}

FBoneNodeData* FBoneNodeData::Find_NodeFromID(_int iID)
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
			FBoneNodeData* pNode = vecChildren[i]->Find_NodeFromID(iID);
			if (pNode != nullptr)
				return pNode;
		}
	}

	return nullptr;
}



// ------------------------ ArmatureData ---------------------------

FArmatureData::FArmatureData(const FArmatureData& rhs)
	: pArmatureNode(rhs.pArmatureNode), mapModelNodeData(rhs.mapModelNodeData)
{
}

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

FArmatureData* FArmatureData::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FBoneNodeData Copy Failed");

		return nullptr;
	}

	if (!pInstance->pArmatureNode)
	{
		mapModelNodeData.clear();
		pInstance->pArmatureNode = Cast<FBoneNodeData*>(pInstance->pArmatureNode->Clone(this));
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

FBoneNodeData* FArmatureData::Find_NodeData(_int iID)
{
	if (iID < 0 || iID >= vecModelNodeIndex.size())
		return nullptr;

	return vecModelNodeIndex[iID];
}

FBoneNodeData* FArmatureData::Find_NodeData(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter == mapModelNodeData.end())
		return nullptr;

	return (*iter).second;
}


FBoneNodeData* FArmatureData::Create_NodeData(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter != mapModelNodeData.end())
		return (*iter).second;

	auto pNode = FBoneNodeData::Create();
	mapModelNodeData.emplace(strModelNodeKey, pNode);

	return pNode;
}

// 아마추어 노드 지정
void FArmatureData::Appoint_ArmatureNode(const wstring& strModelNodeKey)
{
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter == mapModelNodeData.end())
		return;

	pArmatureNode = (*iter).second;
}

HRESULT FArmatureData::Add_NodeData(const wstring& strModelNodeKey, FBoneNodeData* pNode)
{
	if (pNode->iID >= vecModelNodeIndex.size())
		vecModelNodeIndex.resize(pNode->iID + 1, nullptr);

	// ID 중복은 치명적인 오류임
	if (vecModelNodeIndex[pNode->iID] != nullptr)
		return E_FAIL;

	vecModelNodeIndex[pNode->iID] = pNode;

	// 맵에 추가
	auto iter = mapModelNodeData.find(strModelNodeKey);
	if (iter != mapModelNodeData.end())
		return E_FAIL;

	mapModelNodeData.emplace(strModelNodeKey, pNode);

	return S_OK;
}


// ------------------------ ModelNodeGroup ----------------------------


FBoneGroup* FBoneGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void FBoneGroup::Free()
{
	for (auto& Pair : mapArmatureData)
		Safe_Release(Pair.second);
	mapArmatureData.clear();
}

FArmatureData* FBoneGroup::Clone_ArmatureData(const wstring& strArmatureKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second->Clone();
}


FArmatureData* FBoneGroup::Find_ArmatureData(const wstring& strArmatureKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second;
}


FArmatureData* FBoneGroup::Create_ArmatureData(const wstring& strArmatureKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter != mapArmatureData.end())
		return (*iter).second;

	auto pArmature = FArmatureData::Create();
	mapArmatureData.emplace(strArmatureKey, pArmature);

	return pArmature;
}


void FBoneGroup::Appoint_ArmatureNode(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return;

	(*iter).second->Appoint_ArmatureNode(strModelNodeKey);
}


FBoneNodeData* FBoneGroup::Find_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second->Find_NodeData(strModelNodeKey);
}


FBoneNodeData* FBoneGroup::Create_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey)
{
	auto iter = mapArmatureData.find(strArmatureKey);
	if (iter == mapArmatureData.end())
		return nullptr;

	return (*iter).second->Create_NodeData(strModelNodeKey);
}

