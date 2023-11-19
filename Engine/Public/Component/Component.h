#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ��� ������Ʈ�� ����
/// ������ PrimitiveComponent�� ������Ʈ�� �߰��� �� �ִ� �����̴�.
/// ������ ��ŭ �ʿ���� ����� �� ����������, �ʿ��� ���� �������� �����Ͽ� ����.
/// </summary>
class ENGINE_DLL CComponent abstract : public CBase
{
	DERIVED_CLASS(CBase, CComponent)
protected:
	explicit CComponent();
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT		Initialize() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void		Free();

public:
	GETSET_1(wstring, m_strName, Name, GET_C_REF)		// �̸��� ��⸸ ����

private:
	wstring				m_strName;						// ������Ʈ �̸�

public:
	// �⺻�� ������Ʈ��. ���� ������Ʈ�� �߰��� �͵��� PrimiveComponent�� ��ӹ��� ��ü�� ����� ��.
	GETSET_2(CComponent*, m_pOwnerComp, OwnerComp, GET_REF_C, SET__C)

private:
	CComponent*			m_pOwnerComp = { nullptr };

public:
	HRESULT Add_InterComponent(const wstring& strCompKey, CComponent* pComp);
	CComponent* Find_InterComponent(const wstring& strCompKey);

private:
	_unmap<wstring, CComponent*>	m_mapInterComponent;		// ���������� �۵��ϵ��� ����� ������Ʈ�� ���⿡ �ִ´�.
};

END