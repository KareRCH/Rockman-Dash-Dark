#pragma once

#include "Client_Define.h"
#include "BaseClass/GameObject.h"

BEGIN(Engine)
class CPlaneModelComp;
END


BEGIN(Client)


class CTitleScreen : public CGameObject
{
	DERIVED_CLASS(CGameObject, CTitleScreen)

public:
	struct TLoadingScreenDesc
	{
		_float fX;
		_float fY;
		_float fSizeX;
		_float fSizeY;
	};

protected:
	explicit CTitleScreen();
	explicit CTitleScreen(const CTitleScreen& rhs);
	virtual ~CTitleScreen() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	virtual void BeginPlay() override;

public:
	static CTitleScreen* Create();
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void			Free() override;

public:
	virtual FSerialData SerializeData() override;

public:
	HRESULT Initialize_Component();

private:
	CPlaneModelComp* m_pTitleImage = { nullptr };
	CPlaneModelComp* m_pBackgroundImage = { nullptr };


private:
	_float				m_fX, m_fY, m_fSizeX, m_fSizeY;
};

END