#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// �ִϸ��̼��� ������ �� �ִ� ������Ʈ
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent();
	explicit CAnimationComponent(const CAnimationComponent& rhs);
	virtual ~CAnimationComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize(void* Arg = nullptr);

public:
	static CAnimationComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr);

protected:
	virtual void	Free();
};

END