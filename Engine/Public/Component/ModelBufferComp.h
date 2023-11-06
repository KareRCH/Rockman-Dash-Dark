#pragma once

#include "VIBufferComp.h"

BEGIN(Engine)

/// <summary>
/// �𵨿� ���� ���۸� ����ϴ� ������Ʈ
/// �ִϸ��̼Ǳ��� ���������� �����غ�
/// </summary>
class ENGINE_DLL CModelBufferComp : public CVIBufferComp
{
	DERIVED_CLASS(CVIBufferComp, CModelBufferComp)
protected:
	explicit CModelBufferComp(const DX11DEVICE_T tDevice);
	explicit CModelBufferComp(const CModelBufferComp& rhs);
	virtual ~CModelBufferComp() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Initialize(const string& strGroupKey, const string& strModelKey);
	virtual void	PriorityTick() override;
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	LateTick() override;
	virtual void	Render() override;

public:
	static CModelBufferComp* Create(const DX11DEVICE_T tDevice);
	virtual CPrimitiveComponent* Clone(void* Arg);

protected:
	virtual void	Free();
};

END