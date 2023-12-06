#pragma once

#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_Docking final : public Engine::CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Docking)

protected:
	explicit CImGuiWin_Docking() = default;
	explicit CImGuiWin_Docking(const CImGuiWin_Docking& rhs) = default;
	virtual ~CImGuiWin_Docking() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Docking* Create();

private:
	virtual void	Free() override;


private:
	_bool					m_bOpt_FullScreen = true;
	_bool					m_bOpt_Padding;

	ImGuiDockNodeFlags		m_DockSpace_Flags;
	ImGuiWindowFlags		m_Window_Flags;
};

END