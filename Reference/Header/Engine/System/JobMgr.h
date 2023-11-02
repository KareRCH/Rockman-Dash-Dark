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
	_uint	m_iThreadCount = 0;		// ��ü ������ ī��Ʈ
	_uint	m_iJobCount = 0;		// �� ���� ������ ī��Ʈ
	_uint	m_iAsyncCount = 0;		// Async ���� ������ ī��Ʈ
};

END