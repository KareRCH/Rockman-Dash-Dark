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


CBoneGroup::CBoneGroup(const CBoneGroup& rhs)
	: vecBones(rhs.vecBones)
{
	// ���� ����
	for (size_t i = 0; i < vecBones.size(); i++)
	{
		vecBones[i] = vecBones[i]->Clone();
		mapBones.emplace(vecBones[i]->strName, vecBones[i]);
	}

}

CBoneGroup* CBoneGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Create Failed");
	}

	return pInstance;
}

CBoneGroup* CBoneGroup::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Copy Failed");
	}

	return pInstance;
}

void CBoneGroup::Free()
{
	for (auto& Pair : mapBones)
		Safe_Release(Pair.second);
	mapBones.clear();
	vecBones.clear();
}


FBoneData* CBoneGroup::Find_BoneData(_int iID)
{
	if (iID < 0 || iID >= vecBones.size())
		return nullptr;

	return vecBones[iID];
}

FBoneData* CBoneGroup::Find_BoneData(const wstring& strBoneNodeKey)
{
	auto iter = mapBones.find(strBoneNodeKey);
	if (iter == mapBones.end())
		return nullptr;

	return (*iter).second;
}

HRESULT CBoneGroup::Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode)
{
	// ID �߰�
	if (pNode->iID >= vecBones.size())
		vecBones.resize(pNode->iID + 1, nullptr);

	// ID �ߺ��� ġ������ ������
	if (vecBones[pNode->iID] != nullptr)
		return E_FAIL;

	vecBones[pNode->iID] = pNode;


	// �ʿ� �߰�
	auto iter = mapBones.find(strBoneNodeKey);
	if (iter != mapBones.end())
		return E_FAIL;

	mapBones.emplace(strBoneNodeKey, pNode);

	return S_OK;
}

const _float4x4* const CBoneGroup::Provide_BoneFinalTransformPtr(_uint iIndex) const
{
	if (iIndex < 0 || iIndex >= vecBones.size())
		return nullptr;

	return &vecBones[iIndex]->matFinalTransform;
}

HRESULT CBoneGroup::Set_BoneTransform(_uint iIndex, _fmatrix& matTransform)
{
	if (iIndex < 0 || iIndex >= vecBones.size())
		return E_FAIL;

	XMStoreFloat4x4(&vecBones[iIndex]->matTransform, matTransform);

	return S_OK;
}

void CBoneGroup::Invalidate_FinalTransforms()
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
