#pragma once

#include "Tool_Define.h"
#include "BaseClass/ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_Property : public Engine::CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_Property)

protected:
	explicit CImGuiWin_Property() = default;
	explicit CImGuiWin_Property(const CImGuiWin_Property& rhs) = default;
	virtual ~CImGuiWin_Property() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_Property* Create();

private:
	virtual void	Free() override;
};

END