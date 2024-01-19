#pragma once

#include "Base.h"
#include "Client_Define.h"


BEGIN(Engine)
class CGameInstance;
END


BEGIN(Client)

class CMainApp final : public Engine::CBase
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
	void	Render_FrameRate();
	HRESULT Open_Level(LEVEL eStartLevelID);

	HRESULT Open_Level(LEVEL eStartLevelID, const wstring& strLevelPath);

private:		// 접근을 최소화하는 함수, 변수
	CGameInstance* m_pGI = { nullptr };

private:
	_uint			m_iTestLevel = { 0 };

};

END