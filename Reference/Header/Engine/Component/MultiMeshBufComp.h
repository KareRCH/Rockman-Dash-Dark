#pragma once

#include "VIBufferComp.h"

#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)


class FMeshGroup;

struct FMeshBuffer
{
	wstring					strMeshName;
	ComPtr<ID3D11Buffer>	pVtxBuffer;
	ComPtr<ID3D11Buffer>	pIndBuffer;
	_uint					iVtxCount;
	_uint					iIndCount;
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

	// ����� �𵨿� ���� ���ε��Ѵ�. ���� �𵨰� �ٸ��� �޽� ���۰� ���ư���.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

	// ���ε��� ��ģ �� ����� �� �ִ� �Լ���
	// �ϳ��� ��� ���ε�
	HRESULT Add_MeshByOne(const wstring& strMeshKey);
	// ��� �޽� ���ε�
	HRESULT Add_MeshFromModel();

	_uint	Get_MeshesCounts() { return Cast<_uint>(m_vecMeshes.size()); }
	void	Render_Buffer(_uint iBufferIndex);

public:
	static	CMultiMeshBufComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

private:
	wstring					m_strBindedModel = { L"" };
	FMeshGroup*				m_pMeshGroup = { nullptr };
	vector<FMeshBuffer>		m_vecMeshes;
	set<wstring>			m_strBindedMeshs;

};

END