#pragma once

#include "Tool_Define.h"
#include "ImGuiWin.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Tool)

class CImGuiWin_ClassBrowser : public CImGuiWin
{
	DERIVED_CLASS(CImGuiWin, CImGuiWin_ClassBrowser)

protected:
	explicit CImGuiWin_ClassBrowser() = default;
	explicit CImGuiWin_ClassBrowser(const CImGuiWin_ClassBrowser& rhs) = default;
	virtual ~CImGuiWin_ClassBrowser() = default;

public:
	virtual HRESULT	Initialize() override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CImGuiWin_ClassBrowser* Create();

private:
	virtual void	Free() override;

private:
	void Layout_Browser(const _float& fTimeDelta);

public:
	void Create_Object();
	void Save_Prototype(const wstring& strFileName);

public:
	void Set_GameObject(CGameObject* pObj);

private:
	CGameObject* m_pGameObject = { nullptr };

	_int		m_iSelected_Object = -1;

};

END