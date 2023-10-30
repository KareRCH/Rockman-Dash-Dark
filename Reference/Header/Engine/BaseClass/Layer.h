#pragma once

#include "GameObject.h"

#include "BaseClass/GameObject_Enum.h"

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
	static CLayer*	Create(_float fPriority);

private:
	virtual void	Free();

public:
	HRESULT			Ready_Layer(_float fPriority);
	_int			Update_Layer(const _float& fTimeDelta);
	void			LateUpdate_Layer();
	//void			Render_Layer(); 앞으로는 Renderer에서 대신 렌더링을 수행한다.

public:
	CPrimitiveComponent*		Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag);

	

public:
	HRESULT			Add_GameObject(CGameObject* pGameObject);
	HRESULT			Add_GameObject(const wstring pObjTag, CGameObject* pGameObject);
	CGameObject*	Get_GameObject(const wstring pObjTag);

private:
	_unmap<wstring, CGameObject*>	m_mapObject;
	// 렌더는 렌더러에서 해줘서 필요없음! 그래서 사이즈를 렌더로 설정
	vector<CGameObject*>			m_arrvecPriorityObject[Cast_EnumDef(EUPDATE_T::RENDER)];

public:
	GETSET_1(_float, m_fPriority, Priority, GET_C_REF)

protected:
	_float			m_fPriority;


};

END