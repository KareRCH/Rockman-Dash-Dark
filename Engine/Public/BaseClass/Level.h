#pragma once

#include "Layer.h"

BEGIN(Engine)

class CGameInstance;

/// <summary>
/// 기본형 데이터
/// </summary>
class ENGINE_DLL CLevel abstract : public CBase
{
	DERIVED_CLASS(CBase, CLevel)
protected:
	explicit CLevel();
	explicit CLevel(const CLevel& rhs) = delete;
	virtual ~CLevel() = default;

public:
	virtual HRESULT		Initialize() PURE;
	virtual void		Tick(const _float& fTimeDelta) PURE;
	virtual HRESULT		Render() PURE;

protected:
	virtual void		Free();

protected:
	CGameInstance*		m_pGI = { nullptr };

public:
	GETSET_2(wstring, m_strName, Name, GET_C_REF, SET_C_REF)

private:
	wstring				m_strName = TEXT("");

};

END