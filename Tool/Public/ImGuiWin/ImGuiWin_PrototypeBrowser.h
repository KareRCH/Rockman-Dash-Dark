#pragma once

#include "Tool_Define.h"
#include "ImGuiWin.h"

BEGIN(Tool)

class CImGuiWin_PrototypeBrowser : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_PrototypeBrowser)

protected:
	explicit CImGuiWin_PrototypeBrowser() = default;
	explicit CImGuiWin_PrototypeBrowser(const CImGuiWin_PrototypeBrowser& rhs) = default;
	virtual ~CImGuiWin_PrototypeBrowser() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_PrototypeBrowser* Create();

private:
	virtual void	Free() override;
	void Layout_Object(const _float& fTimeDelta);

	void Create_Object();

private:
	vector<string>	m_vecPrototypeNames;
	_int	m_iSelected_Object = -1;
};

END