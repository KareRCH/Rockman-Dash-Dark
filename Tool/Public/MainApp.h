#pragma once

#include "Base.h"
#include "Tool_Define.h"

#include "imgui.h"


BEGIN(Tool)

class CMainApp final : public CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CMainApp)

private:
	explicit CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT	Initialize();
	void	Priority_Tick(const _float& fTimeDelta);
	_int	Tick(const _float& fTimeDelta);
	void	Late_Tick(const _float& fTimeDelta);
	void	Render();

public:
	static CMainApp* Create();

private:
	virtual void Free() override;

private:
	void Render_FrameRate();


private:		// 접근을 최소화하는 함수, 변수
	CGameInstance* m_pGI = { nullptr };
};

END