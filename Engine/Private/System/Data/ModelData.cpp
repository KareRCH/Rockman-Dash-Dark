#include "System/Data/ModelData.h"


HRESULT FModelData::Initialize()
{
	pMeshGroup = CMeshGroup::Create();
	pBoneGroup = CBoneGroup::Create();
	pAnimGroup = CBoneAnimGroup::Create();
	pMaterialGroup = CMaterialGroup::Create();

	return S_OK;
}

FModelData* FModelData::Create(const _bool bLoaded)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("FModelData Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void FModelData::Free()
{
	Safe_Release(pMeshGroup);
	Safe_Release(pMaterialGroup);
	Safe_Release(pBoneGroup);
	Safe_Release(pAnimGroup);
}

FMeshData* FModelData::Find_Mesh(const _uint iIndex)
{
	if (nullptr == pMeshGroup)
		return nullptr;

	return pMeshGroup->Find_Mesh(iIndex);
}

FMeshData* FModelData::Find_Mesh(const wstring& strMesh, const _uint iRangeIndex)
{
	if (nullptr == pMeshGroup)
		return nullptr;

	return pMeshGroup->Find_Mesh(strMesh, iRangeIndex);
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

CBoneGroup* FModelData::Find_BoneGroup()
{
	if (nullptr == pBoneGroup)
		return nullptr;

	return pBoneGroup;
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

