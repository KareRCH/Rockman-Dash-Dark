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


// ------------------------ ArmatureData ---------------------------


FSkeletalData::FSkeletalData(const FSkeletalData& rhs)
	: mapBoneNodes(rhs.mapBoneNodes)
{
}

FSkeletalData* FSkeletalData::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FArmatureData Create Failed");
	}

	return pInstance;
}

FSkeletalData* FSkeletalData::Clone()
{
	ThisClass* pInstance = new ThisClass(*this);

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Copy Failed");
		return nullptr;
	}

	for (size_t i = 0; i < vecBoneNodes.size(); i++)
	{
		vecBoneNodes[i] = vecBoneNodes[i]->Clone();
	}

	return pInstance;
}

void FSkeletalData::Free()
{
	for (auto& Pair : mapBoneNodes)
		Safe_Release(Pair.second);
	mapBoneNodes.clear();
	vecBoneNodes.clear();
}


FBoneData* FSkeletalData::Find_BoneData(_int iID)
{
	if (iID < 0 || iID >= vecBoneNodes.size())
		return nullptr;

	return vecBoneNodes[iID];
}

FBoneData* FSkeletalData::Find_BoneData(const wstring& strBoneNodeKey)
{
	auto iter = mapBoneNodes.find(strBoneNodeKey);
	if (iter == mapBoneNodes.end())
		return nullptr;

	return (*iter).second;
}

HRESULT FSkeletalData::Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode)
{
	// ID �߰�
	if (pNode->iID >= vecBoneNodes.size())
		vecBoneNodes.resize(pNode->iID + 1, nullptr);

	// ID �ߺ��� ġ������ ������
	if (vecBoneNodes[pNode->iID] != nullptr)
		return E_FAIL;

	vecBoneNodes[pNode->iID] = pNode;


	// �ʿ� �߰�
	auto iter = mapBoneNodes.find(strBoneNodeKey);
	if (iter != mapBoneNodes.end())
		return E_FAIL;

	mapBoneNodes.emplace(strBoneNodeKey, pNode);

	return S_OK;
}

vector<_float4x4> FSkeletalData::Provide_FinalTransforms(_bool bNoHierarchi)
{
	vector<_float4x4> vecFinalTransforms;
	vecFinalTransforms.reserve(vecBoneNodes.size());

	if (bNoHierarchi)
		Calculate_FinalTransforms();
	else
		Calculate_FinalTransformsNoHierarchi();

	for (_uint i = 0; i < vecBoneNodes.size(); i++)
	{
		vecFinalTransforms.push_back(vecBoneNodes[i]->matTransform);
	}

	return vecFinalTransforms;
}

void FSkeletalData::Add_Transforms(vector<_float4x4>* pVecMatrices) const
{
	if (pVecMatrices->size() != vecBoneNodes.size())
		return;

	// �������� �ѹ��� clear �� ���¿��� �Ѵ�.
	// ���� ����� Weight�� ������ ���̾�� �Ѵ�.
	for (_uint i = 0; i < vecBoneNodes.size(); i++)
	{
		// ��� ���ϱ�, Weight�� ������ 1�̾�� �Ѵ�.
		XMStoreFloat4x4(&vecBoneNodes[i]->matTransform,
			XMLoadFloat4x4(&(*pVecMatrices)[i]) + XMLoadFloat4x4(&vecBoneNodes[i]->matTransform));
	}
}

void FSkeletalData::Clear_FinalTransforms()
{
	for (_uint i = 0; i < vecBoneNodes.size(); i++)
	{
		XMStoreFloat4x4(&vecBoneNodes[i]->matTransform, {});
	}
}

void FSkeletalData::Calculate_FinalTransforms()
{
	/*OutputDebugString((strName + L"\n").c_str());

	_float4x4 matTemp = {};
	XMStoreFloat4x4(&matTemp, matOffsetTransform * matFinalTransform);

	_float3 vPos = Get_PosFromMatrix(matTemp);
	_float3 vRot = Get_RotEulerFromMatrix(matTemp);
	_float3 vScale = Get_ScaleFromMatrix(matTemp);*/

	_matrix matSkeletalOffset = XMLoadFloat4x4(&matOffset);
	for (_uint i = 0; i < vecBoneNodes.size(); i++)
	{
		FBoneData* pBone = vecBoneNodes[i];

		_matrix matOffsetTransform, matFinalTransform, matParentTransform = XMMatrixIdentity();
		matOffsetTransform = XMLoadFloat4x4(&pBone->matOffset);
		matFinalTransform = XMLoadFloat4x4(&pBone->matTransform);
		if (pBone->iParentID != -1)
			matParentTransform = XMLoadFloat4x4(&vecBoneNodes[pBone->iParentID]->matTransform);

		if (i == 0)
			XMStoreFloat4x4(&pBone->matTransform, (matOffsetTransform * matFinalTransform * matSkeletalOffset));
		else
			XMStoreFloat4x4(&pBone->matTransform, (matOffsetTransform * matFinalTransform * matParentTransform));
	}
}

void FSkeletalData::Calculate_FinalTransformsNoHierarchi()
{
	_matrix matSkeletalOffset = XMLoadFloat4x4(&matOffset);

	for (_uint i = 0; i < vecBoneNodes.size(); i++)
	{
		FBoneData* pBone = vecBoneNodes[i];

		_matrix matOffsetTransform, matFinalTransform;
		matOffsetTransform = XMLoadFloat4x4(&pBone->matOffset);
		matFinalTransform = XMLoadFloat4x4(&pBone->matTransform);

		XMStoreFloat4x4(&pBone->matTransform, (matOffsetTransform * matFinalTransform * matSkeletalOffset));
	}
}


// ------------------------ ModelNodeGroup ----------------------------


FSkeletalGroup* FSkeletalGroup::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		MSG_BOX("FBoneNodeData Create Failed");
	}

	return pInstance;
}

void FSkeletalGroup::Free()
{
	for (auto& Pair : mapSkeletals)
		Safe_Release(Pair.second);
	mapSkeletals.clear();
	vecSkeletals.clear();
}



FSkeletalData* FSkeletalGroup::Create_Skeletal(const wstring& strSkeletalKey)
{
	auto iter = mapSkeletals.find(strSkeletalKey);
	if (iter != mapSkeletals.end())
		return (*iter).second;

	auto pArmature = FSkeletalData::Create();
	mapSkeletals.emplace(strSkeletalKey, pArmature);

	return pArmature;
}


FSkeletalData* FSkeletalGroup::Clone_Skeletal(const wstring& strSkeletalKey)
{
	auto iter = mapSkeletals.find(strSkeletalKey);
	if (iter == mapSkeletals.end())
		return nullptr;

	return (*iter).second->Clone();
}


FSkeletalData* const FSkeletalGroup::Find_Skeletal(const _uint iIndex) const
{
	if (iIndex < 0 && iIndex >= vecSkeletals.size())
		return nullptr;

	return vecSkeletals[iIndex];
}

FSkeletalData* const FSkeletalGroup::Find_Skeletal(const wstring& strName) const
{
	auto iter = mapSkeletals.find(strName);
	if (iter == mapSkeletals.end())
		return nullptr;

	return (*iter).second;
}

