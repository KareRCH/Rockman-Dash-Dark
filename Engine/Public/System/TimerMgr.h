#pragma once

#include "Base.h"
#include "System/Timer.h"


BEGIN(Engine)

/// <summary>
/// 타이머 관리 클래스
/// </summary>
class CTimerMgr final : public CBase
{
	DERIVED_CLASS(CBase, CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr() = default;

public:
	HRESULT				Initialize();

public:
	static CTimerMgr*	Create();

private:
	virtual void		Free();

public:
	HRESULT			Create_Timer(const _tchar* pTimerTag);
	_float			Get_TimeDelta(const _tchar* pTimerTag);
	void			Set_TimeDelta(const _tchar* pTimerTag);

private:
	CTimer*			Find_Timer(const _tchar* pTimerTag) const;

private:
	map<wstring, CTimer*>		m_mapTimers;
};

END