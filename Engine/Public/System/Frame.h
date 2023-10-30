#pragma once

#include <Base.h>

BEGIN(Engine)

class CFrame : public CBase
{
	DERIVED_CLASS(CBase, CFrame)

private:
	explicit CFrame();
	virtual ~CFrame();

public:
	static CFrame* Create(const _float& fCallLimit);
private:
	virtual void		Free();

public:
	_bool			IsPermit_Call(const _float& fTimeDelta);
	HRESULT			Ready_Frame(const _float& fCallLimit);

public:
	GETSET_2(_float, m_fFrameRate, FrameRate, GET_C_REF, SET_C_REF)
	//GETSET_EX2(_float, m_fAccTimeDelta, AccTimeDelta, GET_C_REF, SET_C_REF)
	//GETSET_EX2(_float, m_fCurFrameRate, CurFrameRate, GET_C_REF, SET_C_REF)

private:
	_float			m_fFrameRate;
	_float			m_fCurFrameRate;
	_float			m_fCallLimit;
	_float			m_fAccTimeDelta;


};

END