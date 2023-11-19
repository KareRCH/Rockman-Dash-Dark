#pragma once

#include "Base.h"

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
	virtual HRESULT		Initialize() PURE;

public:
	virtual CComponent* Clone(void* Arg = nullptr) PURE;

protected:
	virtual void		Free();

public:
	GETSET_1(wstring, m_strName, Name, GET_C_REF)		// 이름을 얻기만 가능

private:
	wstring				m_strName;						// 컴포넌트 이름

public:
	// 기본형 컴포넌트임. 게임 오브젝트에 추가된 것들은 PrimiveComponent를 상속받은 객체를 사용할 것.
	GETSET_2(CComponent*, m_pOwnerComp, OwnerComp, GET_REF_C, SET__C)

private:
	CComponent*			m_pOwnerComp = { nullptr };

public:
	HRESULT Add_InterComponent(const wstring& strCompKey, CComponent* pComp);
	CComponent* Find_InterComponent(const wstring& strCompKey);

private:
	_unmap<wstring, CComponent*>	m_mapInterComponent;		// 내부적으로 작동하도록 만드는 컴포넌트는 여기에 넣는다.
};

END