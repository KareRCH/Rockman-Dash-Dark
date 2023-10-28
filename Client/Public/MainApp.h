#pragma once

#include "Base.h"

BEGIN_NAME(Engine)

class CGraphicDev;

END_NAME

class CMainApp : public Engine::CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CMainApp)
private:
	explicit CMainApp();
	virtual ~CMainApp();

public:
	static CMainApp* Create();

public:
	HRESULT Initialize();
	_int	Update();
	void	LateUpdate();
	void	Render();

private:
	virtual void Free() override;


private:		// 접근을 최소화하는 함수, 변수
	Engine::CGraphicDev* m_pDeviceClass;
	ID3D11Device*		 m_pGraphicDev;
};

