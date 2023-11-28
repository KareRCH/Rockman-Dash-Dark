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

	// 사용할 모델에 대해 바인딩한다. 기존 모델과 다르면 메쉬 버퍼가 날아간다.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

	// 바인딩을 마친 뒤 사용할 수 있는 함수들
	// 하나만 골라서 바인딩
	HRESULT Add_MeshByOne(const wstring& strMeshKey);
	// 모든 메쉬 바인딩
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