#pragma once

#include "Base.h"

BEGIN(Engine)

class CInputDev : public CBase
{
	DERIVED_CLASS(CBase, CInputDev)

private:
	explicit CInputDev();
	virtual ~CInputDev() = default;

public:
	HRESULT				Initialize(HINSTANCE hInst, HWND hWnd);
	void				Tick(void);

public:
	static CInputDev*	Create(HINSTANCE hInst, HWND hWnd);

private:
	virtual void		Free(void);

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
	_byte					m_byKeyState[256];		// Ű���忡 �ִ� ��� Ű���� �����ϱ� ���� ����
	DIMOUSESTATE			m_tMouseState;


};

END