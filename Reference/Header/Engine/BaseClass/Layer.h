#pragma once

#include "BaseClass/GameObject.h"

BEGIN(Engine)

/// <summary>
/// ���̾�� GameObject�� ���������� GameObject�� Update �κ��� �����ϴ� Ŭ������
/// ���� ������ ������ ó���ϴ� ���Ҹ��� �����ϴ� ���� �������� �Ѵ�.
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
	// ������ ���������� ���༭ �ʿ����! �׷��� ����� ������ ����
	list<CGameObject*>				m_listPriorityObject;		// Tick ���� �ϳ��� ����

public:
	GETSET_1(_float, m_fPriority, Priority, GET_C_REF)

protected:
	_float			m_fPriority;


};

END