#include "System/GameInstance.h"

#include "System/GraphicDev.h"
#include "System/InputDev.h"
#include "System/KeyMgr.h"
#include "System/PhysicsMgr.h"
#include "System/SoundMgr.h"
#include "System/FrameMgr.h"
#include "System/TimerMgr.h"
#include "System/FontMgr.h"
#include "System/Management.h"
#include "System/BlackBoardMgr.h"
#include "System/TextureMgr.h"
#include "System/ProtoMgr.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize()
{
	return S_OK;
}

void CGameInstance::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pInputDev);
	Safe_Release(m_pKeyMgr);
	Safe_Release(m_pPhysicsMgr);
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pFontMgr);
	Safe_Release(m_pFrameMgr);
	Safe_Release(m_pTimerMgr);
}

HRESULT CGameInstance::Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, 
	_bool bVsync, HWND hWnd, _bool bFullScreen, _float fScreenDepth, _float fScreenNear)
{
	if (nullptr != m_pGraphicDev)
		return E_FAIL;

	NULL_CHECK_RETURN(
		m_pGraphicDev = CGraphicDev::Create(iScreenWidth, iScreenHeight, bVsync, hWnd, bFullScreen, fScreenDepth, fScreenNear)
		, E_FAIL);

	return S_OK;
}

void CGameInstance::Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Render_Begin(fRed, fGreen, fBlue, fAlpha);
}

void CGameInstance::Render_End()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Render_End();
}

ID3D11Device* CGameInstance::Get_GraphicDev()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_Device();
}




#pragma region 입력 디바이스

HRESULT CGameInstance::Initialize_InputDev(HINSTANCE hInst, HWND hWnd)
{
	if (nullptr != m_pInputDev)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pInputDev = CInputDev::Create(hInst, hWnd), E_FAIL);

	return S_OK;
}

void CGameInstance::Tick_InputDev()
{
	if (nullptr == m_pInputDev)
		return;

	m_pInputDev->Tick();
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	if (nullptr == m_pInputDev)
		return (_byte)0;

	return m_pInputDev->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	if (nullptr == m_pInputDev)
		return (_byte)0;

	return m_pInputDev->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	if (nullptr == m_pInputDev)
		return (_byte)0;

	return m_pInputDev->Get_DIMouseMove(eMouseState);
}

#pragma endregion



HRESULT CGameInstance::Initialize_KeyMgr()
{
	if (nullptr != m_pKeyMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pKeyMgr = CKeyMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_PhysicsMgr(_uint iPhysicsWorldCount)
{
	if (nullptr != m_pPhysicsMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pPhysicsMgr = CPhysicsMgr::Create(iPhysicsWorldCount), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_SoundMgr()
{
	if (nullptr != m_pSoundMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pSoundMgr = CSoundMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_FontMgr()
{
	if (nullptr != m_pFontMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pFontMgr = CFontMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_FrameMgr()
{
	if (nullptr != m_pFrameMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pFrameMgr = CFrameMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_TimerMgr()
{
	if (nullptr != m_pTimerMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTimerMgr = CTimerMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_Management(const EMANAGE_SCENE eManageSceneType)
{
	if (nullptr != m_pManagement)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pManagement = CManagement::Create(eManageSceneType), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_BlackBoardMgr()
{
	if (nullptr != m_pBlackBoardMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pBlackBoardMgr = CBlackBoardMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_TextureMgr(ID3D11Device* const pGraphicDev)
{
	if (nullptr != m_pTextureMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTextureMgr = CTextureMgr::Create(pGraphicDev), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Initialize_ProtoMgr()
{
	if (nullptr != m_pManagement)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pProtoMgr = CProtoMgr::Create(), E_FAIL);

	return S_OK;
}
