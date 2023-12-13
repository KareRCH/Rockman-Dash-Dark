#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Engine)
class CTerrain;
END

BEGIN(Tool)

class CImGuiWin_Viewer final : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Viewer)

protected:
	explicit CImGuiWin_Viewer() = default;
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
	void	Mouse_Picking(const _float& fTimeDelta);

private:
	_float2			m_vViewerMin = {};
	_float2			m_vViewerSize = {};
	_float2			m_vViewerCenter = {};
	_bool			m_bMouseOnViewer = false;	// 마우스가 뷰어 안에 있는지.
	_float2			m_vMousePosOnViewer = {};	// 뷰어기준으로 마우스 위치가 변환됨.


	_float			m_fConvertRatio = 1.f;		// 텍스처 복사용
	

private:
	HRESULT			Link_GuiTerrain();
	
private:
	class CImGuiWin_Terrain*	m_pGuiTerrain = { nullptr };	// 연동할 터레인 레이아웃
	class CTerrain*				m_pTerrain = { nullptr };		// 피킹용 터레인
	vector<SHADER_VTX_NORM>		m_TerrainVertices;
	using VERTEX_TYPE = vector<SHADER_VTX_NORM>::value_type;

private:
	class CToolCamera*			m_pCamera = { nullptr };		// 뷰어용 카메라

};

END