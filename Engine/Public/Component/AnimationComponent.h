#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// 애니메이션을 관리할 수 있는 컴포넌트
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