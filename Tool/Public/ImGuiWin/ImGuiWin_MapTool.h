#pragma once

#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_MapTool final : public Engine::CImGuiWin
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

private:
	
	
};

END