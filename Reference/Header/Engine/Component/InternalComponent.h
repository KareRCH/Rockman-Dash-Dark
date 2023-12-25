#pragma once

#include "Component/Component.h"

BEGIN(Engine)

/// <summary>
/// 다른 컴포넌트에 의해 내부적으로 관리되는 컴포넌트.
/// 오브젝트의 요소로써 동작하는 컴포넌트가 바로 내부 컴포넌트라고 한다.
/// 해당 클래스에서는 Primitive에서 쓰이는 많은 Update, Render 함수가 빠져있으며.
/// 필요시 정의하여 사용하도록 권장한다.
/// 그래픽 카드를 필수적으로 사용해야할 때는 PrimInterComp를 사용할 것을 권장한다.
/// Primitive의 기능과 
/// </summary>
class ENGINE_DLL CInternalComponent abstract : public CComponent
{
	DERIVED_CLASS(CComponent, CInternalComponent)
protected:
	explicit CInternalComponent() = default;
	explicit CInternalComponent(const CInternalComponent& rhs);
	virtual ~CInternalComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override PURE;
	virtual HRESULT Initialize(void* Arg = nullptr) override PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void Free();

};

END