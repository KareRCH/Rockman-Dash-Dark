#pragma once

#include "Component/GameObjectComp.h"


BEGIN(Engine)

/// <summary>
/// ���ӿ�����Ʈ�� �߰��� �� �ִ� ������Ʈ�� ����.
/// Primitive Ÿ���� Primitive���� �ʿ� ����ȴ�.
/// 
/// </summary>
class ENGINE_DLL CPrimitiveComponent abstract : public CGameObjectComp
{
	DERIVED_CLASS(CGameObjectComp, CPrimitiveComponent)
protected:
	explicit CPrimitiveComponent() = default;
	explicit CPrimitiveComponent(const CPrimitiveComponent& rhs);
	virtual ~CPrimitiveComponent() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr) PURE;
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual void	Tick(const _float& fTimeDelta) PURE;
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual HRESULT	Render() PURE;

public:
	virtual CComponent*	Clone(void* Arg = nullptr) PURE;

protected:
	virtual void		Free();

};

END


