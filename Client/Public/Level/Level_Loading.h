#pragma once

#include "Client_Define.h"
#include "BaseClass/Level.h"

BEGIN(Client)

/// <summary>
/// 로딩중인 레벨의 데이터를 불러오는 클래스.
/// </summary>
class CLevel_Loading final : public Engine::CLevel
{
	DERIVED_CLASS(CLevel, CLevel_Loading)

private:
	explicit CLevel_Loading();
	explicit CLevel_Loading(const CLevel_Loading& rhs);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT Initialize() { return S_OK; }
	virtual HRESULT Initialize(LEVEL eNextLevelID);
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_Loading*	Create(LEVEL eNextLevelID);
	virtual void			Free() override;

private:
	HRESULT Ready_Objects();

private:
	LEVEL			m_eNextLevelID = { LEVEL_END };
	class CLoader*	m_pLoader = { nullptr };


};

END