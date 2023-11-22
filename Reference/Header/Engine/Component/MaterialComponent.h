#pragma once

#include "InternalComponent.h"

BEGIN(Engine)

/// <summary>
/// 머터리얼을 관리할 수 있는 컴포넌트
/// </summary>
class ENGINE_DLL CMaterialComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CMaterialComponent)
protected:
	explicit CMaterialComponent();
	explicit CMaterialComponent(const CMaterialComponent& rhs);
	virtual ~CMaterialComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CMaterialComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;
};

END