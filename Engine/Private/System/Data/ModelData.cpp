#include "System/Data/ModelData.h"


FModelData* FModelData::Create(const _bool bLoaded)
{
	ThisClass* pInstance = new ThisClass();

	if (!pInstance)
	{
		Safe_Release(pInstance);

		MSG_BOX("FModelData Create Failed");

		return nullptr;
	}

	pInstance->pMeshGroup = FMeshGroup::Create();
	pInstance->pSkeletalGroup = FSkeletalGroup::Create();
	pInstance->pAnimGroup = FBoneAnimGroup::Create();

	return pInstance;
}

void FModelData::Free()
{
	Safe_Release(pMeshGroup);
	Safe_Release(pSkeletalGroup);
	Safe_Release(pAnimGroup);
}

FMeshData* FModelData::Find_Mesh(const _uint iIndex)
{
	if (nullptr == pMeshGroup)
		return nullptr;

	return pMeshGroup->Find_Mesh(iIndex);
}

FMeshData* FModelData::Find_Mesh(const wstring& strMesh)
{
	if (nullptr == pMeshGroup)
		return nullptr;

	return pMeshGroup->Find_Mesh(strMesh);
}

FMaterialData* FModelData::Find_Material(const _uint iIndex)
{
	if (nullptr == pMaterialGroup)
		return nullptr;

	return pMaterialGroup->Find_Material(iIndex);
}

FMaterialData* FModelData::Find_Material(const wstring& strMaterial)
{
	if (nullptr == pMaterialGroup)
		return nullptr;

	return pMaterialGroup->Find_Material(strMaterial);
}

FSkeletalData* FModelData::Find_Skeletal(const _uint iIndex)
{
	if (nullptr == pSkeletalGroup)
		return nullptr;

	return pSkeletalGroup->Find_Skeletal(iIndex);
}

FSkeletalData* FModelData::Find_Skeletal(const wstring& strSkeletal)
{
	if (nullptr == pSkeletalGroup)
		return nullptr;

	return pSkeletalGroup->Find_Skeletal(strSkeletal);
}

FBoneAnimData* FModelData::Find_BoneAnim(const _uint iIndex)
{
	if (nullptr == pAnimGroup)
		return nullptr;

	return pAnimGroup->Find_BoneAnim(iIndex);
}

FBoneAnimData* FModelData::Find_BoneAnim(const wstring& strBoneAnim)
{
	if (nullptr == pAnimGroup)
		return nullptr;

	return pAnimGroup->Find_BoneAnim(strBoneAnim);
}

