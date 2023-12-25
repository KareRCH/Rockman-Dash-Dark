#pragma once

#include "Base.h"


BEGIN(Engine)

struct TMesh;

struct FMeshVertexData
{
	_float3			vPosition;
	_float2			vTexCoord;
	_float3			vNormal;
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
	HRESULT Initialize(const TMesh& MeshData);
	HRESULT Create_Buffer(const TMesh& MeshData);

public:
	static FMeshData*	Create(const TMesh& MeshData);
	virtual void		Free() override;

public:
	// ���� ���� ������Ʈ
	const vector<FMeshBoneData>& Provide_Offsets() const { return vecMeshBoneDatas; }
	
public:
	_uint						iID;					// �޽��� ID

	_uint						iNumVertices = { 0 };	// ���ؽ� ����
	_uint						iVertexStride = { 0 };	// ���ؽ��� ����Ʈ
	ComPtr<ID3D11Buffer>		pVB = { nullptr };		// ���ؽ� ����

	_uint						iNumIndices;			// �ε��� ����
	_uint						iIndexStride = { 0 };	// �ε����� ����Ʈ
	ComPtr<ID3D11Buffer>		pIB = { nullptr };		// �ε��� ����

	DXGI_FORMAT					eIndexFormat = {};		// �ε��� ����
	D3D11_PRIMITIVE_TOPOLOGY	eTopology = { };		// �������� �ɼ�

	_uint						iMaterialIndex;			// ���͸��� �ε���

	vector<FMeshBoneData>		vecMeshBoneDatas;		// ���� ���� ������ �����Ϳ� ���� ���� ID
};



/// <summary>
/// �޽� ������ �׷�
/// </summary>
class CMeshGroup final : public CBase
{
	DERIVED_CLASS(CBase, CMeshGroup)
private:
	explicit CMeshGroup() = default;
	explicit CMeshGroup(const CMeshGroup& rhs) = delete;
	virtual ~CMeshGroup() = default;

public:
	static CMeshGroup* Create();
	virtual void Free() override;
	
public:
	_uint				Get_NumMeshes() const { return m_iNumMeshes; }
	FMeshData* const	Find_Mesh(const _uint iIndex) const;
	FMeshData* const	Find_Mesh(const wstring& strMeshKey, const _uint iRangeIndex) const;
	HRESULT				Add_Mesh(const wstring& strMeshKey, FMeshData* const pMeshData);
	
private:
	_uint							m_iNumMeshes;	// �޽� ����
public:
	multimap<wstring, FMeshData*>	mapMeshDatas;	// �̸� Ž��
	vector<FMeshData*>				vecMeshDatas;	// �ε��� �˻� �޽� ������
};


END