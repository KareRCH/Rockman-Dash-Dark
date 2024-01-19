#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Engine)
class CGameObjectComp;
class CSceneComponent;
class CCommonModelComp;
class CColliderComponent;
END

BEGIN(Tool)

/// <summary>
/// 객체에 설정할 수 있는 속성들을 보여주는 창이다.
/// 기본적으로 위치, 회전, 크기를 볼 수 있고.
/// 추가적으로 설정되는 클래스의 개별 속성들이 있다면, 그것을 설정할 수도 있다.
/// </summary>
class CImGuiWin_Property : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Property)

protected:
	explicit CImGuiWin_Property() = default;
	explicit CImGuiWin_Property(const CImGuiWin_Property& rhs) = default;
	virtual ~CImGuiWin_Property() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Property* Create();

private:
	virtual void	Free() override;

private:
	void Layout_Property();
	void Layout_GameObjectProperty();
	void Layout_GameObjectCompProperty(CGameObjectComp* pComp);
	void Layout_SceneCompProperty(CSceneComponent* pComp);
	void Layout_CommonModelCompProperty(CCommonModelComp* pComp);
	void Layout_ColliderCompProperty(CColliderComponent* pComp);


public:
	void Set_GameObject(CGameObject* pGameObject);
	void Reset_GameObject();

private:
	_bool			m_bChanged_GameObject = { false };
	CGameObject*	m_pGameObject = { nullptr };

};

END