#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

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
};

END