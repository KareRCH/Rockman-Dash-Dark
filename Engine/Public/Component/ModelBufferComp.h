#pragma once

#include "VIBufferComp.h"

#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// �𵨿� ���� ���۸� ����ϴ� ������Ʈ
/// �ִϸ��̼Ǳ��� ���������� �����غ�
/// </summary>
class ENGINE_DLL CModelBufferComp final : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CModelBufferComp)
public:


protected:
	explicit CModelBufferComp() = default;
	explicit CModelBufferComp(const CModelBufferComp& rhs);
	virtual ~CModelBufferComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strModelKey, const _uint iRangeIndex);

	void Render_Buffer();

public:
	static CModelBufferComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

};

END