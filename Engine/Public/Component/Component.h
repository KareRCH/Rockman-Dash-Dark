#pragma once

#include "Base.h"
#include "System/GameInstance.h"

BEGIN(Engine)

/// <summary>
/// 모든 컴포넌트의 원형
/// 하위인 PrimitiveComponent가 오브젝트에 추가할 수 있는 형태이다.
/// 원형인 만큼 필요없는 기능이 다 빠져있으며, 필요한 것은 하위에서 정의하여 쓴다.
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
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)		// 이름을 얻기만 가능

	void		Add_LevelTag(const wstring& strPrototypeTag) { m_setLevelTag.emplace(strPrototypeTag); }
	_bool		Has_LevelTag(const wstring& strPrototypeTag) { return (m_setLevelTag.find(strPrototypeTag) != m_setLevelTag.end()); }
	void		Delete_LevelTag(const wstring& strTag);
	_uint		LevelTag_Size() { return Cast<_uint>(m_setLevelTag.size()); }

private:
	wstring				m_strName;						// 컴포넌트 이름
	set<wstring>		m_setLevelTag;					// 분류태그, 레벨에 따라 로드된 분류에 따라 원본을 정리하는데 사용된다.
protected:
	_bool				m_bIsCloned = { false };		// 원형인지 아닌지

public:
	// 기본형 컴포넌트임. 게임 오브젝트에 추가된 것들은 PrimiveComponent를 상속받은 객체를 사용할 것.
	GETSET_2(CComponent*, m_pParentComp, ParentComp, GET_REF_C, SET__C)

private:
	CComponent*			m_pParentComp = { nullptr };		// 이 컴포넌트를 관리하는 컴포넌트
	vector<CComponent*>	m_pChildrenComp;					// 이 컴포넌트가 관리하는 컴포넌트

protected:
	CGameInstance* m_pGI = { nullptr };
};

END