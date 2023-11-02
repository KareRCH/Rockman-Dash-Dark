#pragma once

#include "Base.h"

#include <thread>
#include <future>
#include <mutex>

BEGIN(Engine)

class CJobMgr final :public CBase
{
	DERIVED_CLASS(CBase, CJobMgr)

private:
	explicit CJobMgr();
	virtual ~CJobMgr() = default;

public:
	HRESULT	Initialize(const _uint iJobCount, const _uint iAsyncCount);

public:
	static CJobMgr* Create(const _uint iJobCount, const _uint iAsyncCount);

private:
	virtual void	Free();

private:
	_uint	m_iThreadCount = 0;		// 전체 쓰레드 카운트
	_uint	m_iJobCount = 0;		// 잡 전용 쓰레드 카운트
	_uint	m_iAsyncCount = 0;		// Async 전용 쓰레드 카운트
};

END