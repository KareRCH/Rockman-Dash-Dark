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
	void	PriorityTick();
	_int	Tick(const _float& fTimeDelta);
	void	LateTick();
	void	Render();

public:
	static CMainApp* Create();

private:
	virtual void Free() override;

private:
	void Render_FrameRate();


private:		// 접근을 최소화하는 함수, 변수
	CGameInstance*			m_pGameInstance = nullptr;
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDeviceContext = nullptr;
};

END