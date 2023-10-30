#pragma once

#include "Base.h"
#include "Client_Define.h"


BEGIN(Client)

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
	_int	Update(const _float& fTimeDelta);
	void	LateUpdate();
	void	Render();

private:
	virtual void Free() override;


private:		// 접근을 최소화하는 함수, 변수
	ID3D11Device*		 m_pGraphicDev = nullptr;
};

END