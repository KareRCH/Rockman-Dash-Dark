#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL_DBG CInputDev final : public CBase
{
	DERIVED_CLASS(CBase, CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev() = default;

public:
	HRESULT				Initialize(HINSTANCE hInst, HWND hWnd);
	void				Tick();
	void				Late_Tick();

public:
	static CInputDev*	Create(HINSTANCE hInst, HWND hWnd);

private:
	virtual void		Free(void);

private:
	HWND	m_hWnd;

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return (m_byKeyState[byKeyID] & 0x80);
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return (m_tMouseState.rgbButtons[eMouse] & 0x80);
	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return (*(((_long*)&m_tMouseState) + eMouseState));
	}

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;


public:
	void		Toggle_LockMouseCenter()
	{
		m_bLockMouseCenter = !m_bLockMouseCenter;
	}
	void		OnOff_LockMouseCenter(_bool bIsOnOff)
	{
		m_bLockMouseCenter = bIsOnOff;
	}

private:
	_bool		m_bLockMouseCenter = false;
};

END