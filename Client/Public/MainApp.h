#pragma once

#include "Base.h"
#include "Client_Define.h"

BEGIN(Engine)

class CGameInstance;

END


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


private:		// ������ �ּ�ȭ�ϴ� �Լ�, ����
	CGameInstance*			m_pGameInstance = nullptr;
	ID3D11Device*			m_pGraphicDev = nullptr;
};

END