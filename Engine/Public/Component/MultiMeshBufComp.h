#pragma once

#include "VIBufferComp.h"

#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)


class FModelData;
class FMeshGroup;

struct FMeshBuffer
{
	_bool					bBinded;
	wstring					strMeshName;
	ComPtr<ID3D11Buffer>	pVtxBuffer;
	ComPtr<ID3D11Buffer>	pIndBuffer;
	_uint					iVtxCount;
	_uint					iIndCount;
};

/// <summary>
/// 복수의 메쉬를 처리 할 수 있는 컴포넌트
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
	// 사용할 모델에 대해 바인딩한다. 기존 모델과 다르면 메쉬 버퍼가 날아간다.
	HRESULT Set_ModelData(FModelData* pModelData);

	// 모델 바인딩을 마친 뒤 사용할 수 있는 함수들, 버퍼를 만들어서 저장한다.
	// 하나만 골라서 바인딩
	HRESULT Bind_Mesh(const wstring& strMeshKey);
	// 모델에 존재하는 모든 메쉬 바인딩
	HRESULT Bind_MeshAll();

	// 등록된 메쉬 언바인드
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// 모든 등록 메쉬 언바인드
	void Unbind_AllMeshes();

	_uint	Get_MeshesCounts() { return Cast<_uint>(m_vecMeshes.size()); }
	void	Render_Buffer(_uint iBufferIndex);

private:
	FModelData*				m_pModelData = { nullptr };
	FMeshGroup*				m_pMeshGroup = { nullptr };
	vector<FMeshBuffer>		m_vecMeshes;
	map<wstring, _uint>		m_strBindedMeshes;

};

END