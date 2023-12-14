#include "System/Data/BoneData.h"

FBoneData::FBoneData(const FBoneData& rhs)
	: iID(rhs.iID)
	, strName(rhs.strName), iParentID(rhs.iParentID)
	, matOffset(rhs.matOffset), matTransform(rhs.matTransform)
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

vector<_float4x4> FBoneGroup::Provide_FinalTransforms(_bool bNoHierarchi)
{
	vector<_float4x4> vecFinalTransforms;
	vecFinalTransforms.reserve(vecBones.size());

	if (bNoHierarchi)
		Calculate_FinalTransforms();
	else
		Calculate_FinalTransformsNoHierarchi();

	for (_uint i = 0; i < vecBones.size(); i++)
	{
		vecFinalTransforms.push_back(vecBones[i]->matTransform);
	}

	return vecFinalTransforms;
}

void FBoneGroup::Add_Transforms(vector<_float4x4>* pVecMatrices) const
{
	if (pVecMatrices->size() != vecBones.size())
		return;

	// 적용전에 한번은 clear 된 상태여야 한다.
	// 받은 행렬은 Weight가 곱해진 값이어야 한다.
	for (_uint i = 0; i < vecBones.size(); i++)
	{
		// 행렬 더하기, Weight가 총합이 1이어야 한다.
		XMStoreFloat4x4(&vecBones[i]->matTransform,
			XMLoadFloat4x4(&(*pVecMatrices)[i]) + XMLoadFloat4x4(&vecBones[i]->matTransform));
	}
}

void FBoneGroup::Clear_FinalTransforms()
{
	for (_uint i = 0; i < vecBones.size(); i++)
	{
		XMStoreFloat4x4(&vecBones[i]->matTransform, {});
	}
}

void FBoneGroup::Calculate_FinalTransforms()
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

		_matrix matOffsetTransform, matFinalTransform, matParentTransform = XMMatrixIdentity();
		matOffsetTransform = XMLoadFloat4x4(&pBone->matOffset);
		matFinalTransform = XMLoadFloat4x4(&pBone->matTransform);
		if (pBone->iParentID != -1)
			matParentTransform = XMLoadFloat4x4(&vecBones[pBone->iParentID]->matTransform);

		XMStoreFloat4x4(&pBone->matTransform, (matOffsetTransform * matFinalTransform * matParentTransform));
	}
}

void FBoneGroup::Calculate_FinalTransformsNoHierarchi()
{
	_matrix matSkeletalOffset = XMMatrixIdentity();//XMLoadFloat4x4(&matOffset);

	for (_uint i = 0; i < vecBones.size(); i++)
	{
		FBoneData* pBone = vecBones[i];

		_matrix matOffsetTransform, matFinalTransform;
		matOffsetTransform = XMLoadFloat4x4(&pBone->matOffset);
		matFinalTransform = XMLoadFloat4x4(&pBone->matTransform);

		XMStoreFloat4x4(&pBone->matTransform, (matOffsetTransform * matFinalTransform * matSkeletalOffset));
	}
}