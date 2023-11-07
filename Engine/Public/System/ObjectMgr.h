#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 생성한 원형 객체를 모아놓는다.
/// 원형객체를 복제하여 생성한 객체를 모아놓는다.
/// 보관하고 있는 사본객체들의 Tick관련 함수를 반복적으로 호출해준다.
/// </summary>
class ENGINE_DLL_DBG CObjectMgr final : public CBase
{
	DERIVED_CLASS(CBase, CObjectMgr)

private:
	explicit CObjectMgr();
	explicit CObjectMgr(const CObjectMgr& rhs) = delete;
	virtual ~CObjectMgr() = default;

public:
	HRESULT				Initialize();
	void				PriorityTick();
	_int				Tick(const _float& fTimeDelta);
	void				LateTick();

public:
	static CObjectMgr*	Create();

private:
	virtual void		Free();

private:
	map<string, class CGameObject*>			m_mapProtoType;

private:
	map<const string, list<class CGameObject*>>			m_mapLayer;
	//map<const string, class CLayer*>					m_mapLayer;
};

END