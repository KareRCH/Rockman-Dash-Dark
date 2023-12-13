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

#pragma region ķ�� ������Ʈ
protected:
	CPipelineComp& PipelineComp() { return (*m_pPipelineComp); }

private:
	CPipelineComp* m_pPipelineComp = { nullptr };
#pragma endregion


private:
	void	Layout_TopBar(const _float& fTimeDelta);
	void	Layout_View(const _float& fTimeDelta);


private:
	array<const _char*, 3>	m_arrCoord_Item = { u8"����", u8"����", u8"���Ϸ�" };
	_uint					m_iCoordItem_Index = 0;

	array<const _char*, 3>	m_arrPivot_Item = { u8"������Ʈ", u8"Ŀ��", u8"�߰���" };
	_uint					m_iPivotItem_Index = 0;

	array<const _char*, 4>	m_arrSnap_Item = { u8"��ǥ", u8"��", u8"��", u8"��"};
	_uint					m_iSnapItem_Index = 0;

private:	// ����
	_bool	m_bSnap_Active = false;

private:		// ��ƿ��Ƽ
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
	_bool			m_bMouseOnViewer = false;	// ���콺�� ��� �ȿ� �ִ���.
	_float2			m_vMousePosOnViewer = {};	// ���������� ���콺 ��ġ�� ��ȯ��.


	_float			m_fConvertRatio = 1.f;		// �ؽ�ó �����
	

private:
	HRESULT			Link_GuiTerrain();
	
private:
	class CImGuiWin_Terrain*	m_pGuiTerrain = { nullptr };	// ������ �ͷ��� ���̾ƿ�
	class CTerrain*				m_pTerrain = { nullptr };		// ��ŷ�� �ͷ���
	vector<SHADER_VTX_NORM>		m_TerrainVertices;
	using VERTEX_TYPE = vector<SHADER_VTX_NORM>::value_type;

private:
	class CToolCamera*			m_pCamera = { nullptr };		// ���� ī�޶�

};

END