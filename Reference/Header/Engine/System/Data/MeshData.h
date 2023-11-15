#pragma once

#include "Base.h"


BEGIN(Engine)

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FMeshData final : public CBase
{
	DERIVED_CLASS(CBase, FMeshData)

private:
	explicit FMeshData() {}
	explicit FMeshData(const FMeshData& rhs) = delete;
	virtual ~FMeshData() = default;

public:
	static FMeshData* Create();
	virtual void Free() override;
	
public:
	struct FVertexBoneData
	{
		wstring			strName;
		vector<_uint>	vecVtxID;
		vector<_float>	vecWeights;
	};

public:
	vector<VERTEX_MODEL>		vecVertices;
	vector<_uint>				vecIndices;
	vector<FVertexBoneData>		vecBone;
	_uint						iMaterIndex;
	_matrix						matTransform;
};



/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FMeshGroup final : public CBase
{
	DERIVED_CLASS(CBase, FMeshGroup)
private:
	explicit FMeshGroup() {}
	virtual ~FMeshGroup() = default;

public:
	static FMeshGroup* Create();
	virtual void Free() override;
	
public:
	const FMeshData* const	Get_Mesh(const wstring& strMeshKey);
	void Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData);
	
public:
	_unmap<wstring, FMeshData*> mapMeshData;	// 메시 저장 맵
};


END