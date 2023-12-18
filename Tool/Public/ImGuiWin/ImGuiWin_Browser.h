#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Tool)


typedef FastDelegate1<_bool> PlaceModeSelectedDelegate;
typedef MulticastDelegate<PlaceModeSelectedDelegate, _bool> PlaceModeSelectedMultiDelegate;

typedef FastDelegate1<CGameObject*> ObjectPlacedDelegate;
typedef MulticastDelegate<ObjectPlacedDelegate, CGameObject*> ObjectPlacedMultiDelegate;

/// <summary>
/// ���� �ҷ����� (������ ������), ������Ʈ ��ġ�� �ϱ� ���� �������� ������ �� �ִ� ��ȣ�ۿ��� �����Ѵ�.
/// </summary>
class CImGuiWin_Browser : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Browser)

protected:
	explicit CImGuiWin_Browser() = default;
	explicit CImGuiWin_Browser(const CImGuiWin_Browser& rhs) = default;
	virtual ~CImGuiWin_Browser() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Browser* Create();

private:
	virtual void	Free() override;

private:
	void Layout_Object(const _float& fTimeDelta);

public:
	// �θ� ������ ����
	void Link_ToMapTool();
	// ������ �ͷ��� GUi�� ����
	void Link_ToTerrainGui();
	// ������ �� ����
	void Link_ToViewer();

	void Handle_TerrainBrushModeSelected(_bool bIsEdit);
	void Handle_PlacePicked(_float3 vPickedWorldPos);

	// �ܺο��� ��ε� ĳ��Ʈ ��������Ʈ�� �������߰�
	void Add_PlaceModeSelectedListener(const PlaceModeSelectedDelegate& Listener) { OnPlaceModeSelected.Add(Listener); }
	void Remove_PlaceModeSelectedListener(const PlaceModeSelectedDelegate& Listener) { OnPlaceModeSelected.Remove(Listener); }

	// �ܺο��� ��ε� ĳ��Ʈ ��������Ʈ�� �������߰�
	void Add_ObjectPlacedListener(const ObjectPlacedDelegate& Listener) { OnObjectPlaced.Add(Listener); }
	void Remove_ObjectPlacedListener(const ObjectPlacedDelegate& Listener) { OnObjectPlaced.Remove(Listener); }

private:
	_bool	m_bIsLinkedToMapTool = false;
	_bool	m_bIsLinkedToTerrainGui = false;
	_bool	m_bIsLinkedToViewer = false;

	PlaceModeSelectedMultiDelegate	OnPlaceModeSelected;		// ��ġ��� ���ý� �̺�Ʈ �߻�
	ObjectPlacedMultiDelegate		OnObjectPlaced;				// ������Ʈ ��ġ���

public:
	// ��ġ ���� �� ����
	enum class EMode { None, Place };

private:
	EMode	m_ePlaceMode = { EMode::None };
	
private:
	_int	m_iSelected_Object = -1;


};

END