#pragma once

#include "Tool_Define.h"
#include "ImGuiWin/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_ObjectTool : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_ObjectTool)

protected:
	explicit CImGuiWin_ObjectTool() = default;
	explicit CImGuiWin_ObjectTool(const CImGuiWin_ObjectTool& rhs) = default;
	virtual ~CImGuiWin_ObjectTool() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_ObjectTool* Create();

private:
	virtual void	Free() override;

private:
	void Layout_MenuBar(const _float& fTimeDelta);
	void Show_SavePrototype();
	void Save_Prototype();

};

END