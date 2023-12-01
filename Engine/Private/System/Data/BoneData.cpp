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

FBoneNodeBaseData* FBoneNodeData::Clone(FSkeletalData* pArg)
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
			pArg->Add_BoneNodeData(pNode->strName, Cast<FBoneNodeData*>(pNode));
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

FSkeletalData::FSkeletalData(const FSkeletalData& rhs)
	: pSkeletalRootNode(rhs.pSkeletalRootNode), mapBoneNodeDatas(rhs.mapBoneNodeDatas)
{
}

FSkeletalData* FSkeletalData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FArmatureData Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

FSkeletalData* FSkeletalData::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	if (!pInstance->pSkeletalRootNode)
	{
		mapBoneNodeDatas.clear();
		pInstance->pSkeletalRootNode = Cast<FBoneNodeData*>(pInstance->pSkeletalRootNode->Clone(this));
	}

	return pInstance;
}

void FSkeletalData::Free()
{
	for (auto& Pair : mapBoneNodeDatas)
	{
		Safe_Release(Pair.second);
	}
	mapBoneNodeDatas.clear();
}

FBoneNodeData* FSkeletalData::Find_BoneNodeData(_int iID)
{
	if (iID < 0 || iID >= vecBoneNodeIndexes.size())
		return nullptr;

	return vecBoneNodeIndexes[iID];
}

FBoneNodeData* FSkeletalData::Find_BoneNodeData(const wstring& strBoneNodeKey)
{
	auto iter = mapBoneNodeDatas.find(strBoneNodeKey);
	if (iter == mapBoneNodeDatas.end())
		return nullptr;

	return (*iter).second;
}

// 아마추어 노드 지정
void FSkeletalData::Appoint_SkeletalRootNode(const wstring& strBoneNodeKey)
{
	auto iter = mapBoneNodeDatas.find(strBoneNodeKey);
	if (iter == mapBoneNodeDatas.end())
		return;

	pSkeletalRootNode = (*iter).second;
}

HRESULT FSkeletalData::Add_BoneNodeData(const wstring& strBoneNodeKey, FBoneNodeData* pNode)
{
	if (pNode->iID >= vecBoneNodeIndexes.size())
		vecBoneNodeIndexes.resize(pNode->iID + 1, nullptr);

	// ID 중복은 치명적인 오류임
	if (vecBoneNodeIndexes[pNode->iID] != nullptr)
		return E_FAIL;

	vecBoneNodeIndexes[pNode->iID] = pNode;

	// 맵에 추가
	auto iter = mapBoneNodeDatas.find(strBoneNodeKey);
	if (iter != mapBoneNodeDatas.end())
		return E_FAIL;

	mapBoneNodeDatas.emplace(strBoneNodeKey, pNode);

	return S_OK;
}

vector<_float4x4> FSkeletalData::Provide_FinalTransforms()
{
	vector<_float4x4> vecFinalTransforms;
	vecFinalTransforms.reserve(vecBoneNodeIndexes.size());

	for (_uint i = 0; i < vecBoneNodeIndexes.size(); i++)
	{
		vecFinalTransforms.push_back(vecBoneNodeIndexes[i]->matTransform);
	}

	return vecFinalTransforms;
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
	for (auto& Pair : mapSkeletaDatas)
		Safe_Release(Pair.second);
	mapSkeletaDatas.clear();
}

FSkeletalData* FBoneGroup::Clone_SkeletalData(const wstring& strSkeletalKey)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter == mapSkeletaDatas.end())
		return nullptr;

	return (*iter).second->Clone();
}


FSkeletalData* FBoneGroup::Find_SkeletalData(const wstring& strSkeletalKey)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter == mapSkeletaDatas.end())
		return nullptr;

	return (*iter).second;
}


FSkeletalData* FBoneGroup::Create_SkeletalData(const wstring& strSkeletalKey)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter != mapSkeletaDatas.end())
		return (*iter).second;

	auto pArmature = FSkeletalData::Create();
	mapSkeletaDatas.emplace(strSkeletalKey, pArmature);

	return pArmature;
}


void FBoneGroup::Appoint_SkeletalRootNode(const wstring& strSkeletalKey, const wstring& strBoneNodeKey)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter == mapSkeletaDatas.end())
		return;

	(*iter).second->Appoint_SkeletalRootNode(strBoneNodeKey);
}


FBoneNodeData* FBoneGroup::Find_BoneNodeData(const wstring& strSkeletalKey, const wstring& strBoneNodeKey)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter == mapSkeletaDatas.end())
		return nullptr;

	return (*iter).second->Find_BoneNodeData(strBoneNodeKey);
}

HRESULT FBoneGroup::Add_BoneNodeData(const wstring& strSkeletalKey, const wstring& strBoneNodeKey, FBoneNodeData* pNode)
{
	auto iter = mapSkeletaDatas.find(strSkeletalKey);
	if (iter == mapSkeletaDatas.end())
		return E_FAIL;

	return (*iter).second->Add_BoneNodeData(strBoneNodeKey, pNode);
}


