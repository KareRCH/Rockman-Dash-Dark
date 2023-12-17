#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)


/// <summary>
/// 맵툴에 필요한 정보들을 담고 그것을 표시하는 클래스이다.
/// 이 클래스에서 필요로 하는 기능들은 다음과 같다.
/// - 계층, 속성, 터레인, 오브젝트 브라우저 등, 하위 윈도우들과 상호작용한다.
/// - 기본적으로 레벨에 배치된 객체들을 알고 있다. 종류는 가리지 않으며, GameObject이면 다 된다.
/// - 터레인, 스카이 박스여도 상관이 없다는 이야기.
/// </summary>
class CImGuiWin_MapTool final : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_MapTool)

protected:
	explicit CImGuiWin_MapTool() = default;
	explicit CImGuiWin_MapTool(const CImGuiWin_MapTool& rhs) = default;
	virtual ~CImGuiWin_MapTool() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_MapTool* Create();

private:
	virtual void	Free() override;


public:
	void Shortcut_Manage();
	void DoMove_PickedObjects(const _float& fTimeDelta);
	void Move_PickedObjects(const _float& fTimeDelta);
	void Rotate_PickedObjects(const _float& fTimeDelta);
	void Scale_PickedObjects(const _float& fTimeDelta);
	void Escape_MovePickedObjects();

public:
	enum class EAction { CancelMove, ConfirmMove };
	ACTION_SET<EAction> m_Actions;

public:
	// 피킹, 배치, 또는 다른 조작
	// 피킹 모드에서는 선택된 물체를 옮길 수 있다.
	enum class EMode { Picking, Place, Other };
	enum class EMoveMode { None, Move, Rotate, Scale };
	enum class EMoveAxis { X, Y, Z, XY, YZ, ZX, ALL };

private:
	EMode						m_eToolMode = { EMode::Picking };
	EMoveMode					m_eMoveMode = { EMoveMode::None };
	EMoveAxis					m_eMoveAxis = { EMoveAxis::ALL };
	vector<class CGameObject*>	m_pPickedObjects;	// 선택된 객체들 (드래그로 객체를 선택할 수 있다.
	
};

END