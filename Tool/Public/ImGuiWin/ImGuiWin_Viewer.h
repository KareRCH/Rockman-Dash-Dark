#pragma once

#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

#include "imgui.h"

BEGIN(Tool)

class CImGuiWin_Viewer final : public Engine::CImGuiWin
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
	_float2			m_vContentSize = {};
	_float			m_fConvertRatio = 1.f;


};

END