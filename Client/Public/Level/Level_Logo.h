#pragma once

#include "Client_Define.h"
#include "BaseClass/Level.h"


BEGIN(Client)

class CLevel_Logo : public CLevel
{
	DERIVED_CLASS(CLevel, CLevel_Logo)

private:
	explicit CLevel_Logo();
	explicit CLevel_Logo(const CLevel_Logo& rhs);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_Logo* Create();
	virtual void		Free() override;

private:
	HRESULT Ready_Objects();

};

END