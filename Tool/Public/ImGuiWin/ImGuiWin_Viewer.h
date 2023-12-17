#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"
#include "Component/PipelineComp.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CTerrain;
END

BEGIN(Tool)

typedef FastDelegate1<CGameObject*> PickedObjectDelegate;
typedef MulticastDelegate<PickedObjectDelegate, CGameObject*> PickedObjectMultiDelegate;

typedef FastDelegate1<_float2> PickedBrushDelegate;
typedef MulticastDelegate<PickedBrushDelegate, _float2> PickedBrushMultiDelegate;

/// <summary>
/// 실제 게임상의 카메라를 통해 화면을 보여준다.
/// 피킹을 할 수 있고, 피킹을 하면 속성, 계층 창에 동기화 된다.
/// </summary>
class CImGuiWin_Viewer final : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Viewer)

protected:
	explicit CImGuiWin_Viewer();
	explicit CImGuiWin_Viewer(const CImGuiWin_Viewer& rhs) = default;
	virtual ~CImGuiWin_Viewer() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Viewer* Create();

private:
	virtual void	Free() override;


private:
	class CGameInstance* m_pGI = { nullptr };

#pragma region 캠뷰 컴포넌트
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion


private:
	void	Layout_TopBar(const _float& fTimeDelta);
	void	Layout_View(const _float& fTimeDelta);


private:
	array<const _char*, 3>	m_arrCoord_Item = { u8"월드", u8"로컬", u8"오일러" };
	_uint					m_iCoordItem_Index = 0;

	array<const _char*, 3>	m_arrPivot_Item = { u8"오브젝트", u8"커서", u8"중간점" };
	_uint					m_iPivotItem_Index = 0;

	array<const _char*, 4>	m_arrSnap_Item = { u8"좌표", u8"점", u8"선", u8"면"};
	_uint					m_iSnapItem_Index = 0;

private:	// 스냅
	_bool	m_bSnap_Active = false;

private:		// 유틸리티
	void	Set_Button_ActiveColor();
	void	Set_Button_NonActiveColor();
	void	Set_Button_ReturnColor();

private:
	ImGuiStyle		m_pStyle;

public:
	void LoadTextureFromFIle(const string& strFileName);


private:
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_pPrevSRV = { nullptr };


private:
	// 마우스로 찍었을 때 어떤 객체를 피킹했는지에 설정한다.
	void	Mouse_Picking(const _float& fTimeDelta);

private:
	_float2			m_vViewerMin = {};
	_float2			m_vViewerSize = {};
	_float2			m_vViewerCenter = {};
	_bool			m_bMouseOnViewer = false;	// 마우스가 뷰어 안에 있는지.
	_float2			m_vMousePosOnViewer = {};	// 뷰어기준으로 마우스 위치가 변환됨.


	_float			m_fConvertRatio = 1.f;		// 텍스처 복사용

public:
	// 피킹되었을 때 어떤 객체에 대한 모드는 이를 통해 설정한다.
	enum class EPickingMode { None, Object, Brush };
	EPickingMode m_ePickingMode = { EPickingMode::Object };
	
	vector<SHADER_VTX_NORM>		m_TerrainVertices;

private:
	class CToolCamera*			m_pCamera = { nullptr };		// 뷰어용 카메라

public:
	// 터레인 에디터에 연결
	void Link_ToTerrainEditor();
	// 터레인 창으로부터 모드 선택을 처리
	void Handle_ModeSelected(_bool bIsEdit);

	// 외부에서 브로드 캐스트 델리게이트에 리스너추가
	void Add_PickedObjectListener(const PickedObjectDelegate& Listener) { OnObjectPicked.Add(Listener); }
	void Remove_PickeObjectListener(const PickedObjectDelegate& Listener) { OnObjectPicked.Remove(Listener); }

	// 외부에서 브로드 캐스트 델리게이트에 리스너추가
	void Add_PickedBrushListener(const PickedBrushDelegate& Listener) { OnBrushPicked.Add(Listener); }
	void Remove_PickedBrushListener(const PickedBrushDelegate& Listener) { OnBrushPicked.Remove(Listener); }

private:
	PickedObjectMultiDelegate	OnObjectPicked;		// 피킹 델리게이트
	PickedBrushMultiDelegate	OnBrushPicked;		// 브러쉬 피킹 델리게이트
	_bool						m_bIsLinkedToTerrainEditor = false;

private:

};

END