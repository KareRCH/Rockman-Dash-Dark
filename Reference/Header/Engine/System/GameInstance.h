#pragma once


#include "Base.h"

BEGIN(Engine)

class CGraphicDev;
class CInputDev;
class CKeyMgr;
class CPhysicsMgr;
class CSoundMgr;
class CFontMgr;
class CFrameMgr;
class CTimerMgr;
class CManagement;

/// <summary>
/// Ŭ���̾�Ʈ���� ������ ����� ����ϱ� ���� �ݵ�� ���ľ��ϴ� ��ü
/// ������ Export_Engine�� ��ü�Ͽ� ���� ���̴�.
/// </summary>
class ENGINE_DLL CGameInstance final : public CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize();

private:
	virtual void Free() override;

public:		// �׷��� ����̽�
	HRESULT Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
									_float fScreenDepth, _float fScreenNear);
	void	Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha);
	void	Render_End();


public:		// ��ǲ ����̽�
	HRESULT Initialize_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Tick_InputDev();
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	

public:		// Ű �Ŵ���
	HRESULT Initialize_KeyMgr();

public:		// ������ �Ŵ���
	HRESULT Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);

public:		// ���� �Ŵ���
	HRESULT Initialize_SoundMgr();

public:		// ��Ʈ �Ŵ���
	HRESULT Initialize_FontMgr();

public:		// ������ �Ŵ���
	HRESULT Initialize_FrameMgr();

public:		// Ÿ�̸� �Ŵ���
	HRESULT Initialize_TimerMgr();

public:
	HRESULT Initialize_Management();

private:
	CGraphicDev*	m_pGraphicDev = nullptr;
	CInputDev*		m_pInputDev = nullptr;
	CKeyMgr*		m_pKeyMgr = nullptr;
	CPhysicsMgr*	m_pPhysicsMgr = nullptr;
	CSoundMgr*		m_pSoundMgr = nullptr;
	CFontMgr*		m_pFontMgr = nullptr;
	CFrameMgr*		m_pFrameMgr = nullptr;
	CTimerMgr*		m_pTimerMgr = nullptr;
};

inline CGameInstance* GameInstance()
{
	return CGameInstance::GetInstance();
}

END