#pragma once

#include "Component/Component.h"

BEGIN(Engine)

/// <summary>
/// �ٸ� ������Ʈ�� ���� ���������� �����Ǵ� ������Ʈ.
/// ������Ʈ�� ��ҷν� �����ϴ� ������Ʈ�� �ٷ� ���� ������Ʈ��� �Ѵ�.
/// �ش� Ŭ���������� Primitive���� ���̴� ���� Update, Render �Լ��� ����������.
/// �ʿ�� �����Ͽ� ����ϵ��� �����Ѵ�.
/// �׷��� ī�带 �ʼ������� ����ؾ��� ���� PrimInterComp�� ����� ���� �����Ѵ�.
/// Primitive�� ��ɰ� 
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