#pragma once

#include "System/Frame.h"

BEGIN(Engine)

class CFrameMgr final : public CBase
{
	DERIVED_CLASS(CBase, CFrameMgr)
private:
	explicit CFrameMgr();
	virtual ~CFrameMgr() = default;

public:
	HRESULT				Initialize();

public:
	static CFrameMgr*	Create();
	
private:
	virtual void	Free();

public:
	HRESULT			Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
	void			Set_FrameRate(const _tchar* pFrameTag, const _float& fFrameRate);
	const _float	Get_FrameRate(const _tchar* pFrameTag);
	_bool			IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	
private:
	CFrame*			Find_Frame(const _tchar* pFrameTag);

private:
	map<const _tchar*, CFrame*>			m_mapFrame;

};

END