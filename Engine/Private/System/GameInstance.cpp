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
	Safe_Release(m_pManagement);
	Safe_Release(m_pBlackBoardMgr);
	Safe_Release(m_pTextureMgr);
	Safe_Release(m_pProtoMgr);
}




//----------------------------------------------------------------


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

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Present();
}

ID3D11Device* CGameInstance::Get_GraphicDev()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_Device();
}

ID3D11DeviceContext* CGameInstance::Get_GraphicContext()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_DeviceContext();
}



//----------------------------------------------------------------




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



//----------------------------------------------------------------



#pragma region 키 매니저(가상키)

HRESULT CGameInstance::Initialize_KeyMgr()
{
	if (nullptr != m_pKeyMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pKeyMgr = CKeyMgr::Create(), E_FAIL);

	return S_OK;
}

void CGameInstance::Tick_KeyMgr()
{
	if (nullptr == m_pKeyMgr)
		return;

	m_pKeyMgr->Tick();
}

void CGameInstance::LateTick_KeyMgr()
{
	if (nullptr == m_pKeyMgr)
		return;

	m_pKeyMgr->LateTick();
}

#pragma endregion



//----------------------------------------------------------------



#pragma region 피직스 매니저

HRESULT CGameInstance::Initialize_PhysicsMgr(_uint iPhysicsWorldCount)
{
	if (nullptr != m_pPhysicsMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pPhysicsMgr = CPhysicsMgr::Create(iPhysicsWorldCount), E_FAIL);

	return S_OK;
}

inline void CGameInstance::StartFrame_PhysicsMgr()
{
	if (nullptr == m_pPhysicsMgr)
		return;

	m_pPhysicsMgr->StartFrame();
}

inline _int CGameInstance::Tick_PhysicsMgr(const Real& fTimeDelta)
{
	if (nullptr == m_pPhysicsMgr)
		return 0;

	m_pPhysicsMgr->Tick(fTimeDelta);

	return 0;
}

inline void CGameInstance::Pause_PhysicsSimulation(const _uint iWorldID)
{
	if (nullptr == m_pPhysicsMgr)
		return;

	m_pPhysicsMgr->Get_World3D(iWorldID)->Pause_Simulation();
}

inline void CGameInstance::Play_PhysicsSimulation(const _uint iWorldID)
{
	if (nullptr == m_pPhysicsMgr)
		return;

	m_pPhysicsMgr->Get_World3D(iWorldID)->Play_Simulation();
}

inline void CGameInstance::Add_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider)
{
	if (nullptr == m_pPhysicsMgr)
		return;

	m_pPhysicsMgr->Get_World3D(iWorldID)->Add_RigidBody(pCollider->pBody);
}

inline void CGameInstance::Delete_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider)
{
	if (nullptr == m_pPhysicsMgr)
		return;

	m_pPhysicsMgr->Get_World3D(iWorldID)->Delete_RigidBody(pCollider->pBody);
}

#pragma endregion




//----------------------------------------------------------------



#pragma region 사운드 매니저

HRESULT CGameInstance::Initialize_SoundMgr()
{
	if (nullptr != m_pSoundMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pSoundMgr = CSoundMgr::Create(), E_FAIL);

	return S_OK;
}

#pragma endregion

//----------------------------------------------------------------


#pragma region 폰트 매니저

HRESULT CGameInstance::Initialize_FontMgr()
{
	if (nullptr != m_pFontMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pFontMgr = CFontMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Create_Font(ID3D11Device* pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	if (nullptr == m_pFontMgr)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pFontMgr->Create_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight), E_FAIL);

	return S_OK;
}

void CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DCOLOR Color)
{
	if (nullptr == m_pFontMgr)
		return;

	m_pFontMgr->Render_Font(pFontTag, pString, pPos, Color);
}

#pragma endregion



//----------------------------------------------------------------


#pragma region 프레임 매니저

HRESULT CGameInstance::Initialize_FrameMgr()
{
	if (nullptr != m_pFrameMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pFrameMgr = CFrameMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	if (nullptr == m_pFrameMgr)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pFrameMgr->Create_Frame(pFrameTag, fCallLimit), E_FAIL);

	return S_OK;
}

_bool CGameInstance::IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	if (nullptr == m_pFrameMgr)
		return false;

	return m_pFrameMgr->IsPermit_Call(pFrameTag, fTimeDelta);
}

const _float CGameInstance::Get_FrameRate(const _tchar* pFrameTag)
{
	if (nullptr == m_pFrameMgr)
		return 0.f;

	return m_pFrameMgr->Get_FrameRate(pFrameTag);
}

#pragma endregion


//----------------------------------------------------------------



#pragma region 타이머 매니저

HRESULT CGameInstance::Initialize_TimerMgr()
{
	if (nullptr != m_pTimerMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTimerMgr = CTimerMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Create_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimerMgr)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pTimerMgr->Create_Timer(pTimerTag), E_FAIL);

	return S_OK;
}

_float CGameInstance::Get_TimerDelta(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimerMgr)
		return 0.f;

	return m_pTimerMgr->Get_TimeDelta(pTimerTag);
}

void CGameInstance::Tick_Timer(const _tchar* pTimerTag)
{
	if (nullptr == m_pTimerMgr)
		return;

	m_pTimerMgr->Set_TimeDelta(pTimerTag);
}

#pragma endregion



//----------------------------------------------------------------



#pragma region 매니지먼트

HRESULT CGameInstance::Initialize_Management(const EMANAGE_SCENE eManageSceneType)
{
	if (nullptr != m_pManagement)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pManagement = CManagement::Create(eManageSceneType), E_FAIL);

	return S_OK;
}

_int CGameInstance::Tick_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	m_pManagement->Tick(fTimeDelta);

	return 0;
}

void CGameInstance::LateTick_Scene()
{
	if (nullptr == m_pManagement)
		return;

	m_pManagement->LateTick();
}

void CGameInstance::Render_Scene(ID3D11Device* pGraphicDev)
{
	if (nullptr == m_pManagement)
		return;

	m_pManagement->Render(pGraphicDev);
}

HRESULT CGameInstance::Set_Scene(CScene* pScene)
{
	if (nullptr == m_pManagement)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pManagement->Set_Scene(pScene), E_FAIL);

	return S_OK;
}

#pragma endregion

//----------------------------------------------------------------


#pragma region 블랙보드 매니저

HRESULT CGameInstance::Initialize_BlackBoardMgr()
{
	if (nullptr != m_pBlackBoardMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pBlackBoardMgr = CBlackBoardMgr::Create(), E_FAIL);

	return S_OK;
}

#pragma endregion



//----------------------------------------------------------------



#pragma region 텍스처 매니저

HRESULT CGameInstance::Initialize_TextureMgr(ID3D11Device* const pGraphicDev)
{
	if (nullptr != m_pTextureMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTextureMgr = CTextureMgr::Create(pGraphicDev), E_FAIL);

	return S_OK;
}

#pragma endregion


//----------------------------------------------------------------



#pragma region 프로토 매니저

HRESULT CGameInstance::Initialize_ProtoMgr()
{
	if (nullptr != m_pProtoMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pProtoMgr = CProtoMgr::Create(), E_FAIL);

	return S_OK;
}

#pragma endregion