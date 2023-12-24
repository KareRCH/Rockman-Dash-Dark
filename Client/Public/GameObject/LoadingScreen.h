#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"


BEGIN(Engine)

class CPlaneModelComp;

END


BEGIN(Client)

/// <summary>
/// 대기중일 때 표시하는 로딩화면
/// </summary>
class CLoadingScreen :public CGameObject
{
	DERIVED_CLASS(CGameObject, CLoadingScreen)

public:
	struct TLoadingScreenDesc
	{
		_float fX;
		_float fY;
		_float fSizeX;
		_float fSizeY;
	};

protected:
	explicit CLoadingScreen();
	explicit CLoadingScreen(const CLoadingScreen& rhs);
	virtual ~CLoadingScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CLoadingScreen*	Create();
	CGameObject*			Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	HRESULT Initialize_Component();

private:
	CPlaneModelComp* m_pPlaneModelComp = { nullptr };

private:
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;


};

END