#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

/// <summary>
/// 추가된 오브젝트에 대해 관리할 수 있는 리스트를 띄워주는 클래스.
/// 오브젝트양에 변화가 생기면 그에 대한 리스트의 변화도 줄 수 있다.
/// 뷰어로부터 피킹을 하면 그에 대한 정보를 받아, 어떤 객체가 선택되었는지 보여준다.
/// 반대로 여기서 선택한 객체는 다른 곳에서도 이벤트가 일어나 선택되었다는 것을 보여준다.
/// 연동된 객체, 속성창과 연동된다.
/// </summary>
class CImGuiWin_Hierarchi : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Hierarchi)

protected:
	explicit CImGuiWin_Hierarchi() = default;
	explicit CImGuiWin_Hierarchi(const CImGuiWin_Hierarchi& rhs) = default;
	virtual ~CImGuiWin_Hierarchi() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Hierarchi* Create();

private:
	virtual void	Free() override;

public:
	void Layout_ObjectList(const _float& fTimeDelta);

public:
	void Pushback_GameObject(CGameObject* pObj);
	void Reset_GameObjectList();
	void Delete_GameObject();

private:
	void Handle_ObjectPlaced(CGameObject* pObj);

	void Link_ToBrowser();

private:
	_bool						m_bIsLinkedToBrowser = false;
	vector<class CGameObject*>	m_vecGameObjects;
	_int						m_iSelected_GameObject = -1;

};

END