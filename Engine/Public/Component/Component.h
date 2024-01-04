#pragma once

#include "Base.h"
#include "System/GameInstance.h"

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
	virtual HRESULT		Initialize_Prototype(void* Arg = nullptr) PURE;
	virtual HRESULT		Initialize(void* Arg = nullptr) PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void		Free();

public:
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)		// �̸��� ��⸸ ����

	void		Add_LevelTag(const wstring& strPrototypeTag) { m_setLevelTag.emplace(strPrototypeTag); }
	_bool		Has_LevelTag(const wstring& strPrototypeTag) { return (m_setLevelTag.find(strPrototypeTag) != m_setLevelTag.end()); }
	void		Delete_LevelTag(const wstring& strTag);
	_uint		LevelTag_Size() { return Cast<_uint>(m_setLevelTag.size()); }

private:
	wstring				m_strName;						// ������Ʈ �̸�
	set<wstring>		m_setLevelTag;					// �з��±�, ������ ���� �ε�� �з��� ���� ������ �����ϴµ� ���ȴ�.
protected:
	_bool				m_bIsCloned = { false };		// �������� �ƴ���

public:
	// �⺻�� ������Ʈ��. ���� ������Ʈ�� �߰��� �͵��� PrimiveComponent�� ��ӹ��� ��ü�� ����� ��.
	GETSET_2(CComponent*, m_pParentComp, ParentComp, GET_REF_C, SET__C)

private:
	CComponent*			m_pParentComp = { nullptr };		// �� ������Ʈ�� �����ϴ� ������Ʈ
	vector<CComponent*>	m_pChildrenComp;					// �� ������Ʈ�� �����ϴ� ������Ʈ

protected:
	CGameInstance* m_pGI = { nullptr };
};

END