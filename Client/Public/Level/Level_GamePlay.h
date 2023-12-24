#pragma once

#include "Client_Define.h"
#include "BaseClass/Level.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
	DERIVED_CLASS(CLevel, CLevel_GamePlay)

private:
	explicit CLevel_GamePlay();
	explicit CLevel_GamePlay(const CLevel_GamePlay& rhs);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_GamePlay* Create();
	virtual void			Free() override;


private:
	HRESULT Ready_Objects();
};

END