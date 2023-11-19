#pragma once

#include "Base.h"


BEGIN(Engine)

struct FMeshVertexData
{
	_float3		vPosition;
	_float3		vNormal;
	_float2		vTexCoord;
	_float3		vTangent;
	_float3		vBiTangent;
	vector<_int>	vecBoneID;
	vector<_float>	vecWeights;
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class ENGINE_DLL FMeshData final : public CBase
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
	vector<FMeshVertexData>		vecVertices;
	vector<_uint>				vecIndices;
	_float4x4					matTransform;
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