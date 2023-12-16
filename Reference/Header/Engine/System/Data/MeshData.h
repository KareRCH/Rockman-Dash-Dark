#pragma once

#include "Base.h"


BEGIN(Engine)


struct FMeshVertexData
{
	_float3			vPosition;
	_float3			vNormal;
	_float2			vTexCoord;
	_float3			vTangent;
	_float3			vBiTangent;
	vector<_int>	vecBoneID;
	vector<_float>	vecWeights;
};

struct FMeshBoneData
{
	_int		iBoneID;
	_float4x4	matOffset;
};

/// <summary>
/// 메쉬의 정보
/// 
/// </summary>
class ENGINE_DLL FMeshData final : public CBase
{
	DERIVED_CLASS(CBase, FMeshData)

private:
	explicit FMeshData() = default;
	explicit FMeshData(const FMeshData& rhs) = delete;
	virtual ~FMeshData() = default;

public:
	static FMeshData* Create();
	virtual void Free() override;

public:
	// 뼈에 대해 업데이트
	const vector<FMeshBoneData>& Provide_Offsets() const { return vecMeshBoneDatas; }
	
public:
	_uint						iID;				// 메쉬의 ID
	vector<FMeshVertexData>		vecVertices;		// 정점 데이터
	vector<_uint>				vecIndices;			// 인덱스 데이터
	_uint						iMaterialIndex;		// 머터리얼 인덱스

	vector<FMeshBoneData>		vecMeshBoneDatas;			// 뼈에 대한 오프셋 데이터와 뼈에 대한 ID
};



/// <summary>
/// 메쉬 데이터 그룹
/// </summary>
class FMeshGroup final : public CBase
{
	DERIVED_CLASS(CBase, FMeshGroup)
private:
	explicit FMeshGroup() = default;
	explicit FMeshGroup(const FMeshGroup& rhs) = delete;
	virtual ~FMeshGroup() = default;

public:
	static FMeshGroup* Create();
	virtual void Free() override;
	
public:
	FMeshData* const	Find_Mesh(const _uint iIndex) const;
	FMeshData* const	Find_Mesh(const wstring& strMeshKey) const;
	HRESULT				Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData);
	
public:
	_unmap<wstring, FMeshData*> mapMeshDatas;	// 이름 탐색
	vector<FMeshData*>			vecMeshDatas;	// 인덱스 검색 메쉬 데이터
};


END