#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ������ ���� ��ü�� ��Ƴ��´�.
/// ������ü�� �����Ͽ� ������ ��ü�� ��Ƴ��´�.
/// �����ϰ� �ִ� �纻��ü���� Tick���� �Լ��� �ݺ������� ȣ�����ش�.
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