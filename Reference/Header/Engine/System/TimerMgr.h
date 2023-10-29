#pragma once

#include "Base.h"
#include "System/Timer.h"


BEGIN_NAME(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)
private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float			Get_TimeDelta(const _tchar* pTimerTag);
	void			Set_TimeDelta(const _tchar* pTimerTag);

public:
	HRESULT			Ready_Timer(const _tchar* pTimerTag);

private:
	CTimer* Find_Timer(const _tchar* pTimerTag) const;

private:
	map<const _tchar*, CTimer*>		m_mapTimers;

public:
	virtual void	Free();
};

END_NAME