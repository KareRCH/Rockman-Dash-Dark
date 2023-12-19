#pragma once

#include "Base.h"
#include "ModelBinary_Define.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


BEGIN(Engine)
class CGameInstance;
END


BEGIN(ModelBinary)

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

private:		// ������ �ּ�ȭ�ϴ� �Լ�, ����
	CGameInstance* m_pGI = { nullptr };

};

END