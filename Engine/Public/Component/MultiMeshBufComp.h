#pragma once

#include "VIBufferComp.h"

#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)


class FModelData;
class CMeshGroup;
class CBoneGroup;

struct FMeshBuffer
{
	_bool					bBinded;
	wstring					strMeshName;
	ComPtr<ID3D11Buffer>	pVtxBuffer;
	ComPtr<ID3D11Buffer>	pIndBuffer;
	_uint					iVtxCount;
	_uint					iIndCount;
	const _float4x4*		matTransform;
};

/// <summary>
/// ������ �޽��� ó�� �� �� �ִ� ������Ʈ
/// </summary>
class ENGINE_DLL CMultiMeshBufComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CMultiMeshBufComp)
protected:
	explicit CMultiMeshBufComp() = default;
	explicit CMultiMeshBufComp(const CMultiMeshBufComp& rhs);
	virtual ~CMultiMeshBufComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static	CMultiMeshBufComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;


public:
	// ����� �𵨿� ���� ���ε��Ѵ�. ���� �𵨰� �ٸ��� �޽� ���۰� ���ư���.
	HRESULT Set_ModelData(FModelData* pModelData);

	// �� ���ε��� ��ģ �� ����� �� �ִ� �Լ���, ���۸� ���� �����Ѵ�.
	// �ϳ��� ��� ���ε�
	HRESULT Bind_Mesh(const wstring& strMeshKey, const _uint iRangeIndex);
	// �𵨿� �����ϴ� ��� �޽� ���ε�
	HRESULT Bind_MeshAll();

	// ��ϵ� �޽� ����ε�
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// ��� ��� �޽� ����ε�
	void Unbind_AllMeshes();

	// �޽��� ���� ��İ� ����� ���� ���� ����� ���̴��� ���ε� ���ش�.
	HRESULT Bind_MeshOffsetsToEffect(class CEffectComponent* pEffect, const _char* pConstantName, _uint iMeshIndex,  const CBoneGroup& pBoneGroup);

	_uint	Get_MeshesCounts() { return Cast<_uint>(m_vecMeshes.size()); }
	_float4x4	Get_MeshTransform(_uint iIndex) { return *m_vecMeshes[iIndex].matTransform; }
	void	Bind_Buffer(_uint iBufferIndex);
	void	Render_Buffer(_uint iBufferIndex);

private:
	FModelData*				m_pModelData = { nullptr };
	CMeshGroup*				m_pMeshGroup = { nullptr };
	vector<FMeshBuffer>		m_vecMeshes;
	map<wstring, _uint>		m_strBindedMeshes;

};

END