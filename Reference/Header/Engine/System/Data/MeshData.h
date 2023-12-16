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
/// �޽��� ����
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
	// ���� ���� ������Ʈ
	const vector<FMeshBoneData>& Provide_Offsets() const { return vecMeshBoneDatas; }
	
public:
	_uint						iID;				// �޽��� ID
	vector<FMeshVertexData>		vecVertices;		// ���� ������
	vector<_uint>				vecIndices;			// �ε��� ������
	_uint						iMaterialIndex;		// ���͸��� �ε���

	vector<FMeshBoneData>		vecMeshBoneDatas;			// ���� ���� ������ �����Ϳ� ���� ���� ID
};



/// <summary>
/// �޽� ������ �׷�
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
	_unmap<wstring, FMeshData*> mapMeshDatas;	// �̸� Ž��
	vector<FMeshData*>			vecMeshDatas;	// �ε��� �˻� �޽� ������
};


END