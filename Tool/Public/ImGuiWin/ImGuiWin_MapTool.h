#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)


/// <summary>
/// ������ �ʿ��� �������� ��� �װ��� ǥ���ϴ� Ŭ�����̴�.
/// �� Ŭ�������� �ʿ�� �ϴ� ��ɵ��� ������ ����.
/// - ����, �Ӽ�, �ͷ���, ������Ʈ ������ ��, ���� �������� ��ȣ�ۿ��Ѵ�.
/// - �⺻������ ������ ��ġ�� ��ü���� �˰� �ִ�. ������ ������ ������, GameObject�̸� �� �ȴ�.
/// - �ͷ���, ��ī�� �ڽ����� ����� ���ٴ� �̾߱�.
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
	// ��ŷ, ��ġ, �Ǵ� �ٸ� ����
	// ��ŷ ��忡���� ���õ� ��ü�� �ű� �� �ִ�.
	enum class EMode { Picking, Place, Other };
	enum class EMoveMode { None, Move, Rotate, Scale };
	enum class EMoveAxis { X, Y, Z, XY, YZ, ZX, ALL };

private:
	EMode						m_eToolMode = { EMode::Picking };
	EMoveMode					m_eMoveMode = { EMoveMode::None };
	EMoveAxis					m_eMoveAxis = { EMoveAxis::ALL };
	vector<class CGameObject*>	m_pPickedObjects;	// ���õ� ��ü�� (�巡�׷� ��ü�� ������ �� �ִ�.
	
};

END