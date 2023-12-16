#include "System/Data/BoneData.h"

FBoneData::FBoneData(const FBoneData& rhs)
	: iID(rhs.iID)
	, strName(rhs.strName), iParentID(rhs.iParentID)
	, matTransform(rhs.matTransform), matFinalTransform(rhs.matFinalTransform)
{
}

FBoneData* FBoneData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Create Failed");
		return nullptr;
	}

	return pInstance;
}

FBoneData* FBoneData::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Copy Failed");
	}

	return pInstance;
}

void FBoneData::Free()
{

}


// ------------------------ ModelNodeGroup ----------------------------


FBoneGroup::FBoneGroup(const FBoneGroup& rhs)
	: mapBones(rhs.mapBones), vecBones(rhs.vecBones)
{
	// 깊은 복사
	for (size_t i = 0; i < vecBones.size(); i++)
		vecBones[i] = vecBones[i]->Clone();
}

FBoneGroup* FBoneGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Create Failed");
	}

	return pInstance;
}

FBoneGroup* FBoneGroup::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Copy Failed");
		return nullptr;
	}

	return pInstance;
}

void FBoneGroup::Free()
{
	for (auto& Pair : mapBones)
		Safe_Release(Pair.second);
	mapBones.clear();
	vecBones.clear();
}


FBoneData* FBoneGroup::Find_BoneData(_int iID)
{
	if (iID < 0 || iID >= vecBones.size())
		return nullptr;

	return vecBones[iID];
}

FBoneData* FBoneGroup::Find_BoneData(const wstring& strBoneNodeKey)
{
	auto iter = mapBones.find(strBoneNodeKey);
	if (iter == mapBones.end())
		return nullptr;

	return (*iter).second;
}

HRESULT FBoneGroup::Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode)
{
	// ID 추가
	if (pNode->iID >= vecBones.size())
		vecBones.resize(pNode->iID + 1, nullptr);

	// ID 중복은 치명적인 오류임
	if (vecBones[pNode->iID] != nullptr)
		return E_FAIL;

	vecBones[pNode->iID] = pNode;


	// 맵에 추가
	auto iter = mapBones.find(strBoneNodeKey);
	if (iter != mapBones.end())
		return E_FAIL;

	mapBones.emplace(strBoneNodeKey, pNode);

	return S_OK;
}

const _float4x4* const FBoneGroup::Provide_BoneFinalTransformPtr(_uint iIndex) const
{
	if (iIndex < 0 || iIndex >= vecBones.size())
		return nullptr;

	return &vecBones[iIndex]->matFinalTransform;
}

HRESULT FBoneGroup::Set_BoneTransform(_uint iIndex, _fmatrix& matTransform)
{
	if (iIndex < 0 || iIndex >= vecBones.size())
		return E_FAIL;

	XMStoreFloat4x4(&vecBones[iIndex]->matTransform, matTransform);

	return S_OK;
}

void FBoneGroup::Invalidate_FinalTransforms()
{
	/*OutputDebugString((strName + L"\n").c_str());

	_float4x4 matTemp = {};
	XMStoreFloat4x4(&matTemp, matOffsetTransform * matFinalTransform);

	_float3 vPos = Get_PosFromMatrix(matTemp);
	_float3 vRot = Get_RotEulerFromMatrix(matTemp);
	_float3 vScale = Get_ScaleFromMatrix(matTemp);*/

	for (_uint i = 0; i < vecBones.size(); i++)
	{
		FBoneData* pBone = vecBones[i];

		_matrix matTransform, matParentFinalTransform = XMMatrixIdentity();
		matTransform = XMLoadFloat4x4(&pBone->matTransform);
		if (pBone->iParentID != -1)
			matParentFinalTransform = XMLoadFloat4x4(&vecBones[pBone->iParentID]->matFinalTransform);

		XMStoreFloat4x4(&pBone->matFinalTransform, (matTransform * matParentFinalTransform));
	}
}
