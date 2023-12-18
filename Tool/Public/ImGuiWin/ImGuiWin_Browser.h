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
/// 레벨 불러오기 (저장은 별개로), 오브젝트 배치를 하기 위해 아이템을 선택할 수 있는 상호작용을 제공한다.
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
	// 부모 맵툴에 연결
	void Link_ToMapTool();
	// 형제중 터레인 GUi에 연결
	void Link_ToTerrainGui();
	// 형제중 뷰어에 연결
	void Link_ToViewer();

	void Handle_TerrainBrushModeSelected(_bool bIsEdit);
	void Handle_PlacePicked(_float3 vPickedWorldPos);

	// 외부에서 브로드 캐스트 델리게이트에 리스너추가
	void Add_PlaceModeSelectedListener(const PlaceModeSelectedDelegate& Listener) { OnPlaceModeSelected.Add(Listener); }
	void Remove_PlaceModeSelectedListener(const PlaceModeSelectedDelegate& Listener) { OnPlaceModeSelected.Remove(Listener); }

	// 외부에서 브로드 캐스트 델리게이트에 리스너추가
	void Add_ObjectPlacedListener(const ObjectPlacedDelegate& Listener) { OnObjectPlaced.Add(Listener); }
	void Remove_ObjectPlacedListener(const ObjectPlacedDelegate& Listener) { OnObjectPlaced.Remove(Listener); }

private:
	_bool	m_bIsLinkedToMapTool = false;
	_bool	m_bIsLinkedToTerrainGui = false;
	_bool	m_bIsLinkedToViewer = false;

	PlaceModeSelectedMultiDelegate	OnPlaceModeSelected;		// 배치모드 선택시 이벤트 발생
	ObjectPlacedMultiDelegate		OnObjectPlaced;				// 오브젝트 배치모드

public:
	// 배치 할지 안 할지
	enum class EMode { None, Place };

private:
	EMode	m_ePlaceMode = { EMode::None };
	
private:
	_int	m_iSelected_Object = -1;


};

END