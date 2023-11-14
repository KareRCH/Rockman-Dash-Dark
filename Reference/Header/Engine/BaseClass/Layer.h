#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

/// <summary>
/// 레이어는 GameObject를 저장하지만 GameObject의 Update 부분을 수행하는 클래스로
/// 게임 로직과 물리를 처리하는 역할만을 수행하는 것을 목적으로 한다.
/// </summary>
class ENGINE_DLL CLayer : public CBase
{
	DERIVED_CLASS(CBase, CLayer)
private:
	explicit CLayer();
	virtual ~CLayer() = default;

public:
	HRESULT			Initialize(_float fPriority);
	void			Priority_Tick(const _float& fTimeDelta);
	_int			Tick(const _float& fTimeDelta);
	void			Late_Tick(const _float& fTimeDelta);

public:
	static CLayer* Create(_float fPriority);

private:
	virtual void	Free();

public:
	CPrimitiveComponent*		Get_Component(COMPONENTID eID, const wstring& pObjTag, const wstring& pComponentTag);

public:
	HRESULT			Add_GameObject(CGameObject* pGameObject);
	HRESULT			Add_GameObject(const wstring& pObjTag, CGameObject* pGameObject);
	CGameObject*	Get_GameObject(const wstring& pObjTag);

private:
	_unmap<wstring, CGameObject*>	m_mapObject;
	// 렌더는 렌더러에서 해줘서 필요없음! 그래서 사이즈를 렌더로 설정
	list<CGameObject*>				m_listPriorityObject;		// Tick 순서 하나로 통일

public:
	GETSET_1(_float, m_fPriority, Priority, GET_C_REF)

protected:
	_float			m_fPriority;


};

END