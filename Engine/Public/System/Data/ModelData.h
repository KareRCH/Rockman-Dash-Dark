#pragma once

#include "Base.h"

#include "System/Data/MeshData.h"
#include "System/Data/BoneData.h"
#include "System/Data/BoneAnimData.h"
#include "System/Data/MaterialData.h"

BEGIN(Engine)

/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class ENGINE_DLL FModelData final : public CBase
{
	DERIVED_CLASS(CBase, FModelData)
private:
	explicit FModelData() = default;
	explicit FModelData(const FModelData& rhs) = delete;
	virtual ~FModelData() = default;

public:
	HRESULT Initialize();

public:
	static FModelData* Create(const _bool bLoaded);

private:
	virtual void Free() override;

public:
	FMeshData*		Find_Mesh(const _uint iIndex);
	FMeshData*		Find_Mesh(const wstring& strMesh, const _uint iRangeIndex);

	FMaterialData*	Find_Material(const _uint iIndex);
	FMaterialData*	Find_Material(const wstring& strMaterial);

	CBoneGroup*		Find_BoneGroup();

	FBoneAnimData*	Find_BoneAnim(const _uint iIndex);
	FBoneAnimData*	Find_BoneAnim(const wstring& strBoneAnim);

public:
	class CMeshGroup*		pMeshGroup = { nullptr };			// �޽��� ��Ƴ��� �׷�
	class CMaterialGroup*	pMaterialGroup = { nullptr };		// ���� ���� �׷�
	class CBoneGroup*		pBoneGroup = { nullptr };		// �� ���� �׷�
	class CBoneAnimGroup*	pAnimGroup = { nullptr };			// �ִϸ��̼� �׷�
	
};

END