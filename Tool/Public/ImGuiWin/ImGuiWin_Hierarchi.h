#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_Hierarchi : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Hierarchi)

protected:
	explicit CImGuiWin_Hierarchi() = default;
	explicit CImGuiWin_Hierarchi(const CImGuiWin_Hierarchi& rhs) = default;
	virtual ~CImGuiWin_Hierarchi() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Hierarchi* Create();

private:
	virtual void	Free() override;
};

END