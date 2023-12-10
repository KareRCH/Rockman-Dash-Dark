#pragma once

#include "Client_Define.h"
#include "BaseClass/Level.h"

BEGIN(Client)

class CLevel_Loading final : public Engine::CLevel
{
	DERIVED_CLASS(CLevel, CLevel_Loading)

private:
	explicit CLevel_Loading() = default;
	explicit CLevel_Loading(const CLevel_Loading& rhs);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(const _float& fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	class CLoader* m_pLoader = { nullptr };

public:
	static CLevel_Loading* Create();
	virtual void Free() override;
};

END