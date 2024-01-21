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

#pragma region ķ�� ������Ʈ
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

public:
	void Layout_MenuBar(const _float& fTimeDelta);

public:
	void Shortcut_Manage();
	void DoMove_PickedObjects(const _float& fTimeDelta);
	void Move_PickedObjects(const _float& fTimeDelta, class CGameObject* pObj, _uint iIndex);
	void Rotate_PickedObjects(const _float& fTimeDelta, class CGameObject* pObj, _uint iIndex);
	void Scale_PickedObjects(const _float& fTimeDelta, class CGameObject* pObj, _uint iIndex);
	void Escape_MovePickedObjects();

	void Save_Level(const wstring& strSavePath);
	void Load_Level(const wstring& strLoadPath);

public:
	enum class EAction { CancelMove, ConfirmMove };
	ACTION_SET<EAction> m_Actions;

public:
	// ��ŷ, ��ġ, �Ǵ� �ٸ� ����
	// ��ŷ ��忡���� ���õ� ��ü�� �ű� �� �ִ�.
	enum class EMode { Picking, Place, Other };
	enum class EMoveMode { None, Move, Rotate, Scale };
	enum class EMoveAxis { X, Y, Z, XY, YZ, ZX, ALL };
	enum class ESnapMode { Increment, Ray };	// ��ǥ�������, ���� ���� ���

public:
	void Set_GameObject(class CGameObject* pObj);
	void Add_GameObject(class CGameObject* pObj);
	void Clear_GameObjects();

private:
	_bool						m_bIsSnap = { false };
	EMode						m_eToolMode = { EMode::Picking };
	EMoveMode					m_eMoveMode = { EMoveMode::None };
	EMoveAxis					m_eMoveAxis = { EMoveAxis::ALL };
	vector<class CGameObject*>	m_pPickedObjects;	// ���õ� ��ü�� (�巡�׷� ��ü�� ������ �� �ִ�.

private:
	enum ETransformIndex { Origin, Dest, TI_END };
	_bool	m_bIsMoveFirst = { true };
	_float3 m_vPosition[TI_END] = {};
	_float3 m_vRotation[TI_END] = {};
	_float3 m_vScale[TI_END] = {};

	_float2 m_vMousePos[TI_END] = {};

	vector<_float3x3> m_vTransforms;		// ���� ������Ʈ ���� Ʈ������
	
public:

};

END