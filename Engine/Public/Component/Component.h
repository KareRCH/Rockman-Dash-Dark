#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ��� ������Ʈ�� ����
/// ������ PrimitiveComponent�� ������Ʈ�� �߰��� �� �ִ� �����̴�.
/// </summary>
class ENGINE_DLL CComponent abstract : public CBase
{
	DERIVED_CLASS(CBase, CComponent)
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

public:
	virtual CComponent* Clone(void* Arg) PURE;

protected:
	virtual void					Free() PURE;
};

END