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
enum class EMANAGE_SCENE : _uint;
class CBlackBoardMgr;
class CTextureMgr;
class CProtoMgr;
class CRenderMgr;

/// <summary>
/// 클라이언트에서 엔진의 기능을 사용하기 위해 반드시 거쳐야하는 객체
/// 기존의 Export_Engine을 대체하여 사용될 것이다.
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

public:		// 그래픽 디바이스
	HRESULT Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
									_float fScreenDepth, _float fScreenNear);
	void	Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha);
	void	Render_End();
	ID3D11Device* Get_GraphicDev();
	


public:		// 인풋 디바이스
	HRESULT Initialize_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Tick_InputDev();
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	

public:		// 키 매니저
	HRESULT Initialize_KeyMgr();

public:		// 피직스 매니저
	HRESULT Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);

public:		// 사운드 매니저
	HRESULT Initialize_SoundMgr();

public:		// 폰트 매니저
	HRESULT Initialize_FontMgr();

public:		// 프레임 매니저
	HRESULT Initialize_FrameMgr();

public:		// 타이머 매니저
	HRESULT Initialize_TimerMgr();

public:		// 매니지먼트
	HRESULT Initialize_Management(const EMANAGE_SCENE eManageSceneType);

public:		// 블랙보드 매니저
	HRESULT Initialize_BlackBoardMgr();

public:		// 텍스처 매니저
	HRESULT Initialize_TextureMgr(ID3D11Device* const pGraphicDev);

public:		// 프로토 매니저
	HRESULT Initialize_ProtoMgr();

private:
	CGraphicDev*	m_pGraphicDev = nullptr;
	CInputDev*		m_pInputDev = nullptr;
	CKeyMgr*		m_pKeyMgr = nullptr;
	CPhysicsMgr*	m_pPhysicsMgr = nullptr;
	CSoundMgr*		m_pSoundMgr = nullptr;
	CFontMgr*		m_pFontMgr = nullptr;
	CFrameMgr*		m_pFrameMgr = nullptr;
	CTimerMgr*		m_pTimerMgr = nullptr;
	CManagement*	m_pManagement = nullptr;
	CBlackBoardMgr* m_pBlackBoardMgr = nullptr;
	CTextureMgr*	m_pTextureMgr = nullptr;
	CProtoMgr*		m_pProtoMgr = nullptr;
};

inline CGameInstance* GameInstance()
{
	return CGameInstance::GetInstance();
}

END