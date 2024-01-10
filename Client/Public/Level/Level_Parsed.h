#pragma once

#include "Client_Define.h"
#include "BaseClass/Level.h"

BEGIN(Engine)

class CLevel_Parsed : public CLevel
{
	DERIVED_CLASS(CLevel, CLevel_Parsed)

private:
	explicit CLevel_Parsed();
	explicit CLevel_Parsed(const CLevel_Parsed& rhs);
	virtual ~CLevel_Parsed() = default;

public:
	virtual HRESULT Initialize() { return S_OK; }
	virtual HRESULT Initialize(const wstring& strParsedLevelDataPath);
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CLevel_Parsed*	Create(const wstring& strParsedLevelDataPath);
	virtual void			Free() override;

private:
	HRESULT Ready_Objects(class FSerialData& Data);
};

END