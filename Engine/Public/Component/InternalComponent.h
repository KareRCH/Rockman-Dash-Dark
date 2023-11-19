#pragma once

#include "PrimitiveComponent.h"

BEGIN(Engine)

/// <summary>
/// �ٸ� ������Ʈ�� ���� ���������� �����Ǵ� ������Ʈ.
/// ������Ʈ�� ��ҷν� �����ϴ� ������Ʈ�� �ٷ� ���� ������Ʈ��� �Ѵ�.
/// </summary>
class CInternalComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CInternalComponent)
protected:
	explicit CInternalComponent(const DX11DEVICE_T tDevice);
	explicit CInternalComponent(const CInternalComponent& rhs);
	virtual ~CInternalComponent() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg) PURE;

protected:
	virtual void Free();
};

END