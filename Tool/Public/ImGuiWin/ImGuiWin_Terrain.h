#pragma once


#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"
#include "Component/D3D11DeviceComp.h"
#include "Component/PipelineComp.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)
class CTerrain;
END

BEGIN(Tool)

typedef FastDelegate1<_bool> ModeSelectedDelegate;
typedef MulticastDelegate<ModeSelectedDelegate, _bool> ModeSeleectedBroadDelegate;

/// <summary>
/// 터레인을 생성, 수정, 높이, 브러쉬를 통해 수정을 가할 수 있는 클래스
/// </summary>
class CImGuiWin_Terrain : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Terrain)

protected:
	explicit CImGuiWin_Terrain();
	explicit CImGuiWin_Terrain(const CImGuiWin_Terrain& rhs) = default;
	virtual ~CImGuiWin_Terrain() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Terrain* Create();

private:
	virtual void	Free() override;

private:
	class CGameInstance* m_pGI = { nullptr };

#pragma region 디바이스 컴포넌트
private:
	ID3D11Device* D3D11Device() const { return m_pDeviceComp->Get_Device(); }
	ID3D11DeviceContext* D3D11Context() const { return m_pDeviceComp->Get_Context(); }

private:
	CD3D11DeviceComp* m_pDeviceComp = { nullptr };
#pragma endregion

#pragma region 파이프라인 컴포넌트
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion

private:
	void		Layout_TerrainCreate(const _float& fTimeDelta);
	void		Layout_TerrainEdit(const _float& fTimeDelta);
	void		Layout_TerrainDraw(const _float& fTimeDelta);
	void		Layout_TerrainHeight(const _float& fTimeDelta);

	HRESULT		Terrain_SaveFile();

public:
	GETSET_1(CTerrain*, m_pPickedTerrain, Terrain, GET__C)

private:
	class CTerrain*				m_pPickedTerrain = { nullptr };		// 현재 터레인

	_int						m_ivTerrainVertex_CountX = 129;
	_int						m_ivTerrainVertex_CountZ = 129;

	_int						m_ivTerrainMaxWidth = 100;

	class CTerrain*				m_pPrevPickedTerrain = { nullptr };	// 정점 데이터 유지 체크를 위한 이전 터레인 저장 변수
	vector<SHADER_VTX_NORM>		m_TerrainVertices;					// 터레인 정점
	_uint						m_iTerrainVertexCountX;		
	_uint						m_iTerrainVertexCountZ;
	vector<_uint>				m_TerrainIndices;					// 터레인 인덱스 버퍼

public:
	// 뷰어에 연결 대기하는 함수
	HRESULT Link_ToViewer();
	// 브라우저에 연결 대기하는 함수
	void Link_ToBrowser();

private:
	_bool		m_bIsLinkedToViewer = { false };
	_bool		m_bIsLinkedToBrowser = { false };

public:
	void Handle_PickedObject(CGameObject* pGameObj);
	void Handle_PickedBrush(_float3 vPickedWorldPos);
	void Handle_PlaceModeSelected(_bool bIsSelected);
	_bool IsCanEdit_Terrain() { return (m_bIsEditing && nullptr != m_pPickedTerrain); }

	// 외부에서 브로드 캐스트 델리게이트에 리스너추가
	void Add_ModeSelectedListener(const ModeSelectedDelegate& Listener) { OnModeSelected.Add(Listener); }
	void Remove_ModeSelectedListener(const ModeSelectedDelegate& Listener) { OnModeSelected.Remove(Listener); }

public:
	// 편집모드, 생성, 편집, 브러쉬 모드가 있다.
	enum class EEditMode { Create, Edit, Draw, Height };
	// 브러시 모드
	enum class EBrushType { Circle, Rect };

private:
	_bool		m_bIsEditing = { false };
	EEditMode	m_eEditMode = { EEditMode::Create };
	EBrushType	m_eBrushType = { EBrushType::Circle };

	ModeSeleectedBroadDelegate OnModeSelected;
	_bool						m_bIsClickedOnViewer = false;
	_float3						m_vPickedWorldPos = {};

private:	// 그리기 브러쉬 설정값들
	_float	m_fDrawBrush_Diameter = 3.f;		// 브러쉬 사이즈, 지름 기준


private:
	void HeightBrush_Draw(const _float& fTimeDelta);

private:	// 브러쉬 설정값들
	_bool	m_bIsSetting_HeightBrush = false;	// 브러쉬 세팅중임
	_float	m_fHeightBrush_Diameter = 3.f;		// 브러쉬 사이즈, 지름 기준
	_float	m_fHeightBrush_Strength = 1.f;		// 브러쉬 강한 정도
	

	
};

END