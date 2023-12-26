#include "System/GameInstance.h"

#include "System/GraphicDev.h"
#include "System/InputDev.h"
#include "System/KeyMgr.h"
#include "System/PhysicsMgr.h"
#include "System/SoundMgr.h"
#include "System/FrameMgr.h"
#include "System/TimerMgr.h"
#include "System/FontMgr.h"
#include "System/PipelineMgr.h"

#include "System/LevelMgr.h"
#include "System/ObjectMgr.h"
#include "System/ComponentMgr.h"

#include "System/CloudStationMgr.h"
#include "System/TextureMgr.h"
#include "System/RenderMgr.h"
#include "System/ModelMgr.h"
#include "System/ParticleMgr.h"

#include "BaseClass/GameObject.h"
#include "Component/Component.h"


IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize(HINSTANCE hInst, HWND hWnd, FDEVICE_INIT tDeviceInit)
{
	// 공통되는 시스템을 로드한다.
	FAILED_CHECK_RETURN(Initialize_GraphicDev(tDeviceInit), E_FAIL);

	DX11DEVICE_T tDevice = { Get_GraphicDev().Get(), Get_GraphicContext().Get() };

	FAILED_CHECK_RETURN(Initialize_InputDev(hInst, hWnd), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_RenderMgr(tDevice), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_PipelineMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_PhysicsMgr(1), E_FAIL);

	FAILED_CHECK_RETURN(Initialize_KeyMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_FrameMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_TimerMgr(), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_FontMgr(tDevice), E_FAIL);
	FAILED_CHECK_RETURN(Initialize_CloudStationMgr(), E_FAIL);

	FAILED_CHECK_RETURN(Initialize_ComponentMgr(), E_FAIL);
	Add_PrototypeComp(L"GraphicDevComp", CD3D11DeviceComp::Create());
	Add_PrototypeComp(L"CamViewComp", CPipelineComp::Create());

	FAILED_CHECK_RETURN(Initialize_ObjectMgr(), E_FAIL);
	

	FAILED_CHECK_RETURN(Initialize_LevelMgr(), E_FAIL);

	return S_OK;
}

void CGameInstance::Clear(const wstring& strLevelTag)
{
	if (nullptr == m_pObjectMgr
		|| nullptr == m_pComponentMgr)
		return;

	m_pObjectMgr->Clear_GameObject(strLevelTag);
}

void CGameInstance::Free()
{
	
}

void CGameInstance::Release_Managers()
{
	Safe_Release(m_pModelMgr);
	Safe_Release(m_pRenderMgr);
	Safe_Release(m_pComponentMgr);
	Safe_Release(m_pObjectMgr);
	Safe_Release(m_pLevelMgr);
	Safe_Release(m_pBlackBoardMgr);


	Safe_Release(m_pParticleMgr);
	Safe_Release(m_pShaderMgr);


	Safe_Release(m_pFontMgr);
	Safe_Release(m_pTextureMgr);
	Safe_Release(m_pPipelineMgr);


	Safe_Release(m_pPhysicsMgr);
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pFrameMgr);
	Safe_Release(m_pTimerMgr);
	Safe_Release(m_pKeyMgr);

	Safe_Release(m_pInputDev);
	Safe_Release(m_pGraphicDev);
}




//----------------------------------------------------------------


HRESULT CGameInstance::Initialize_GraphicDev(const FDEVICE_INIT& tInit)
{
	if (nullptr != m_pGraphicDev)
		return E_FAIL;

	NULL_CHECK_RETURN(
		m_pGraphicDev = CGraphicDev::Create(tInit)
		, E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Present();
}

ComPtr<ID3D11Device> CGameInstance::Get_GraphicDev()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_Device();
}

ComPtr<ID3D11DeviceContext> CGameInstance::Get_GraphicContext()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_DeviceContext();
}

void CGameInstance::TurnOn_ZBuffer()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->TurnOn_ZBuffer();
}

void CGameInstance::TurnOff_ZBuffer()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->TurnOff_ZBuffer();
}

void CGameInstance::TurnOn_Cull()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->TurnOn_Cull();
}

void CGameInstance::TurnOff_Cull()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->TurnOff_Cull();
}

HRESULT CGameInstance::Resize_SwapChain(_uint iWidth, _uint iHeight)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Resize_SwapChain(iWidth, iHeight);
}

HRESULT CGameInstance::Regist_RenderTarget(_uint iRenderTargetIndex)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Regist_RenderTarget(iRenderTargetIndex);
}

void CGameInstance::Bind_RenderTargetsOnDevice()
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Bind_RenderTargetsOnDevice();
}

void CGameInstance::Add_SystemViewport(D3D11_VIEWPORT Viewport)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Add_Viewport(Viewport);
}

void CGameInstance::Set_SystemViewport(_uint iIndex, D3D11_VIEWPORT Viewport)
{
	if (nullptr == m_pGraphicDev)
		return;

	m_pGraphicDev->Set_Viewport(iIndex, Viewport);
}

D3D11_VIEWPORT* CGameInstance::Get_SystemViewportPtr(_uint iIndex)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_ViewportPtr(iIndex);
}

HRESULT CGameInstance::Bind_SystemViewport()
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Bind_Viewport();
}

HRESULT CGameInstance::Bind_SystemViewport(_uint iIndex)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Bind_Viewport(iIndex);
}

const D3D11_TEXTURE2D_DESC* CGameInstance::Get_RTV_Desc(_uint iIndex)
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return m_pGraphicDev->Get_RTV_Desc(iIndex);
}

void CGameInstance::Set_RTV_Desc(_uint iIndex, D3D11_TEXTURE2D_DESC& Desc)
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Set_RTV_Desc(iIndex, Desc);
}

HRESULT CGameInstance::Copy_RenderTargetViewToTexture_ByViewport(ComPtr<ID3D11Texture2D>& pTexture, _uint iRenderTargetIndex, _uint iViewportIndex)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Copy_RenderTargetViewToTexture_ByViewport(pTexture, iRenderTargetIndex, iViewportIndex);
}

HRESULT CGameInstance::Copy_BackBufferToTexture_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, _uint iViewportIndex)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Copy_BackBufferToSRV_ByViewport(pSRV, iViewportIndex);
}

HRESULT CGameInstance::Copy_BackBufferToTexture_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, const D3D11_VIEWPORT Viewport)
{
	if (nullptr == m_pGraphicDev)
		return E_FAIL;

	return m_pGraphicDev->Copy_BackBufferToSRV_ByViewport(pSRV, Viewport);
}

const _float CGameInstance::ResolutionRatio()
{
	if (nullptr == m_pGraphicDev)
		return 1.f;

	return m_pGraphicDev->Get_ResolutionRatio();
}

const _uint CGameInstance::ResolutionX()
{
	if (nullptr == m_pGraphicDev)
		return 0;

	return m_pGraphicDev->Get_ResolutionX();
}

const _uint CGameInstance::ResolutionY()
{
	if (nullptr == m_pGraphicDev)
		return 0;

	return m_pGraphicDev->Get_ResolutionX();
}

void CGameInstance::Set_Resolution(_uint iX, _uint iY)
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Set_Resolution(iX, iY);
}

void CGameInstance::Set_ResolutionX_MaintainRatio(_uint iX)
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Set_ResolutionX_MaintainRatio(iX);
}

void CGameInstance::Set_ResolutionY_MaintainRatio(_uint iY)
{
	if (nullptr == m_pGraphicDev)
		return;

	return m_pGraphicDev->Set_ResolutionY_MaintainRatio(iY);
}

const _float CGameInstance::ResolutionByScreen_RatioX()
{
	if (nullptr == m_pGraphicDev)
		return 1.f;

	return m_pGraphicDev->Get_ResolutionByScreen_RatioX();
}

const _float CGameInstance::ResolutionByScreen_RatioY()
{
	if (nullptr == m_pGraphicDev)
		return 1.f;

	return m_pGraphicDev->Get_ResolutionByScreen_RatioX();
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

void CGameInstance::Late_Tick_InputDev()
{
	if (nullptr == m_pInputDev)
		return;

	m_pInputDev->Late_Tick();
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

void CGameInstance::Toggle_LockMouseCenter()
{
	if (nullptr == m_pInputDev)
		return;

	m_pInputDev->Toggle_LockMouseCenter();
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

void CGameInstance::Late_Tick_KeyMgr()
{
	if (nullptr == m_pKeyMgr)
		return;

	m_pKeyMgr->Late_Tick();
}

inline _bool CGameInstance::IsKey_Pressing(const int& iKey)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Key_Pressing(iKey);
}

inline _bool CGameInstance::IsKey_Pressed(const int& iKey)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Key_Down(iKey);
}

inline _bool CGameInstance::IsKey_Released(const int& iKey)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Key_Up(iKey);
}

inline _bool CGameInstance::IsMouse_Pressing(const MOUSEKEYSTATE& iMouse)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Mouse_Pressing(iMouse);
}

inline _bool CGameInstance::IsMouse_Pressed(const MOUSEKEYSTATE& iMouse)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Mouse_Down(iMouse);
}

inline _bool CGameInstance::IsMouse_Released(const MOUSEKEYSTATE& iMouse)
{
	if (nullptr == m_pKeyMgr)
		return false;

	return m_pKeyMgr->Mouse_Up(iMouse);
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

HRESULT CGameInstance::Initialize_SoundMgr(const string& strMainPath)
{
	if (nullptr != m_pSoundMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pSoundMgr = CSoundMgr::Create(strMainPath), E_FAIL);

	return S_OK;
}

void CGameInstance::Play_Sound(const wstring& strGroupKey, const wstring& strSoundKey, CHANNELID eID, _float fVolume)
{
	if (nullptr == m_pSoundMgr)
		return;

	m_pSoundMgr->Play_Sound(strGroupKey, strSoundKey, eID, fVolume);
}

void CGameInstance::Play_BGM(const wstring& strGroupKey, const wstring& strSoundKey, _float fVolume)
{
	if (nullptr == m_pSoundMgr)
		return;

	m_pSoundMgr->Play_BGM(strGroupKey, strSoundKey, fVolume);
}

void CGameInstance::Stop_Sound(CHANNELID eID)
{
	if (nullptr == m_pSoundMgr)
		return;

	m_pSoundMgr->Stop_Sound(eID);
}

void CGameInstance::Stop_SoundAll()
{
	if (nullptr == m_pSoundMgr)
		return;

	m_pSoundMgr->Stop_All();
}

void CGameInstance::Set_ChannelVolume(CHANNELID eID, _float fVolume)
{
	if (nullptr == m_pSoundMgr)
		return;

	m_pSoundMgr->Set_ChannelVolume(eID, fVolume);
}

#pragma endregion

//----------------------------------------------------------------


#pragma region 폰트 매니저

HRESULT CGameInstance::Initialize_FontMgr(const DX11DEVICE_T tDevice)
{
	if (nullptr != m_pFontMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pFontMgr = CFontMgr::Create(tDevice), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Create_Font(const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	if (nullptr == m_pFontMgr)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pFontMgr->Create_Font(pFontTag, pFontType, iWidth, iHeight, iWeight), E_FAIL);

	return S_OK;
}

void CGameInstance::Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2* pPos, D3DCOLOR Color)
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


#pragma region 캠뷰 매니저

HRESULT CGameInstance::Initialize_PipelineMgr()
{
	if (nullptr != m_pPipelineMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pPipelineMgr = CPipelineMgr::Create(), E_FAIL);

	return S_OK;
}

void CGameInstance::Tick_PipelineMgr()
{
	if (nullptr == m_pPipelineMgr)
		return;

	m_pPipelineMgr->Tick();
}

CPipelineMgr* CGameInstance::Get_PipelineMgr()
{
	return m_pPipelineMgr;
}

#pragma endregion




//----------------------------------------------------------------

#pragma region 레벨 매니저

HRESULT CGameInstance::Initialize_LevelMgr()
{
	if (nullptr != m_pLevelMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pLevelMgr = CLevelMgr::Create(), E_FAIL);

	return S_OK;
}

void CGameInstance::Tick_LevelMgr(const _float& fTimeDelta)
{
	if (nullptr == m_pLevelMgr)
		return;

	return m_pLevelMgr->Tick(fTimeDelta);
}

HRESULT CGameInstance::Open_Level(_uint iCurrentLevelIndex, CLevel* pNewLevel)
{
	if (nullptr == m_pLevelMgr)
		return E_FAIL;

	return m_pLevelMgr->Open_Level(iCurrentLevelIndex, pNewLevel);
}
#pragma endregion


//----------------------------------------------------------------


#pragma region 오브젝트 매니저

HRESULT CGameInstance::Initialize_ObjectMgr()
{
	if (nullptr != m_pObjectMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pObjectMgr = CObjectMgr::Create(0), E_FAIL);

	return S_OK;
}

void CGameInstance::Priority_Tick_Object(const _float& fTimeDelta)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Priority_Tick(fTimeDelta);
}

_int CGameInstance::Tick_Object(const _float& fTimeDelta)
{
	if (nullptr == m_pObjectMgr)
		return -1;

	m_pObjectMgr->Tick(fTimeDelta);

	return 0;
}

void CGameInstance::Late_Tick_Object(const _float& fTimeDelta)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Late_Tick(fTimeDelta);
}

HRESULT CGameInstance::Add_PrototypeObject(const wstring& strPrototypeKey, CGameObject* pPrototype)
{
	if (nullptr == m_pObjectMgr)
		return E_FAIL;

	return m_pObjectMgr->Add_CloneObject(strPrototypeKey, pPrototype);
}

HRESULT CGameInstance::Add_CloneObject(const wstring& strPrototypeKey, void* pArg)
{
	if (nullptr == m_pObjectMgr)
		return E_FAIL;

	return m_pObjectMgr->Add_CloneObject(strPrototypeKey, pArg);
}

CGameObject* CGameInstance::Find_PrototypeObject(const wstring& strPrototypeKey)
{
	if (nullptr == m_pObjectMgr)
		return nullptr;

	return m_pObjectMgr->Find_Prototype(strPrototypeKey);
}

void CGameInstance::Clear_PrototypeObejcts(const wstring& strContainTag)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Clear_Prototypes(strContainTag);
}

void CGameInstance::Set_LevelTag(const wstring& strLevelTag)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Set_LevelTag(strLevelTag);
}

HRESULT CGameInstance::Add_GameObject(CGameObject* pObj)
{
	if (nullptr == m_pObjectMgr)
		return E_FAIL;

	return m_pObjectMgr->Add_GameObject(pObj);
}

CGameObject* CGameInstance::Find_GameObjectByID(_uint iFindID)
{
	if (nullptr == m_pObjectMgr)
		return nullptr;

	return m_pObjectMgr->Find_GameObjectByID(iFindID);
}

CGameObject* CGameInstance::Find_GameObjectByIndex(_uint iIndex)
{
	if (nullptr == m_pObjectMgr)
		return nullptr;

	return m_pObjectMgr->Find_GameObjectByIndex(iIndex);
}

void CGameInstance::Clear_GameObject(const wstring& strLayerTag)
{
	if (nullptr == m_pObjectMgr)
		return;

	m_pObjectMgr->Clear_GameObject(strLayerTag);
}

#pragma endregion

//----------------------------------------------------------------


#pragma region 블랙보드 매니저

HRESULT CGameInstance::Initialize_CloudStationMgr()
{
	if (nullptr != m_pBlackBoardMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pBlackBoardMgr = CCloudStationMgr::Create(), E_FAIL);

	return S_OK;
}

#pragma endregion



//----------------------------------------------------------------



#pragma region 텍스처 매니저

HRESULT CGameInstance::Initialize_TextureMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	if (nullptr != m_pTextureMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTextureMgr = CTextureMgr::Create(tDevice, strMainPath), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::IsReady_TextureMgr()
{
	if (nullptr == m_pTextureMgr)
		return E_FAIL;

	return S_OK;
}

HRESULT CGameInstance::Load_Texture(const wstring& strFileName, const _uint iNumTextures, const _bool bPermanent, const _bool bUseMainPath)
{
	if (nullptr == m_pTextureMgr)
		return E_FAIL;

	return m_pTextureMgr->Load_Texture(strFileName, iNumTextures, bPermanent, bUseMainPath);
}

ID3D11ShaderResourceView* CGameInstance::Find_SRV(const wstring& strTextureKey, const _uint iIndex)
{
	if (nullptr == m_pTextureMgr)
		return nullptr;

	return m_pTextureMgr->Find_SRV(strTextureKey, iIndex);
}

const wstring CGameInstance::Get_TextureMainPath()
{
	if (nullptr == m_pTextureMgr)
		return TEXT("");

	return m_pTextureMgr->Get_MainPath();
}

CTextureMgr* CGameInstance::Get_TextureMgr()
{
	return m_pTextureMgr;
}

#pragma endregion


//----------------------------------------------------------------



#pragma region 프로토 매니저

HRESULT CGameInstance::Initialize_ComponentMgr()
{
	if (nullptr != m_pComponentMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pComponentMgr = CComponentMgr::Create(), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Add_PrototypeComp(const wstring& strProtoKey, CComponent* pPrototype)
{
	if (nullptr == m_pComponentMgr)
		return E_FAIL;

	return m_pComponentMgr->Add_Prototype(strProtoKey, pPrototype);
}

CComponent* CGameInstance::Clone_PrototypeComp(const wstring& strProtoKey, void* pArg)
{
	if (nullptr == m_pComponentMgr)
		return nullptr;

	return m_pComponentMgr->Clone_Prototype(strProtoKey, pArg);
}

CComponent* CGameInstance::Reference_PrototypeComp(const wstring& strProtoKey)
{
	if (nullptr == m_pComponentMgr)
		return nullptr;

	return m_pComponentMgr->Reference_Prototype(strProtoKey);
}

void CGameInstance::Clear_PrototypeComps(const wstring& strContainTag)
{
	if (nullptr == m_pComponentMgr)
		return;

	m_pComponentMgr->Clear_Prototypes(strContainTag);
}


#pragma endregion


//-----------------------------------------------------


#pragma region 렌더 매니저

HRESULT CGameInstance::Initialize_RenderMgr(const DX11DEVICE_T tDevice, const _uint iWidth, const _uint iHeight)
{
	if (nullptr != m_pRenderMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pRenderMgr = CRenderMgr::Create(tDevice), E_FAIL);

	return S_OK;
}

void CGameInstance::Render()
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Render();
}

void CGameInstance::Add_RenderGroup(ERenderGroup eType, CGameObject* pGameObject)
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Add_RenderGroup(eType, pGameObject);
}

void CGameInstance::Clear_RenderGroup()
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Clear_RenderGroup();
}

void CGameInstance::Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPers)
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Set_PerspectiveViewMatrix(iCam, matPers);
}

const _matrix CGameInstance::Get_PerspectiveViewMatrix(const _uint iCam) const
{
	if (nullptr == m_pRenderMgr)
		return _matrix();

	return m_pRenderMgr->Get_PerspectiveViewMatrix(iCam);
}

void CGameInstance::Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj)
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Set_PerspectiveProjMatrix(iCam, matPersProj);
}

const _matrix CGameInstance::Get_PerspectiveProjMatrix(const _uint iCam) const
{
	if (nullptr == m_pRenderMgr)
		return _matrix();

	return m_pRenderMgr->Get_PerspectiveProjMatrix(iCam);
}

void CGameInstance::Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrtho)
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Set_OrthogonalViewMatrix(iCam, matOrtho);
}

const _matrix CGameInstance::Get_OrthogonalViewMatrix(const _uint iCam) const
{
	if (nullptr == m_pRenderMgr)
		return _matrix();

	return m_pRenderMgr->Get_OrthogonalViewMatrix(iCam);
}

void CGameInstance::Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matOrthoProj)
{
	if (nullptr == m_pRenderMgr)
		return;

	m_pRenderMgr->Set_OrthogonalProjMatrix(iCam, matOrthoProj);
}

const _matrix CGameInstance::Get_OrthogonalProjMatrix(const _uint iCam) const
{
	if (nullptr == m_pRenderMgr)
		return _matrix();

	return m_pRenderMgr->Get_OrthogonalProjMatrix(iCam);
}

#pragma endregion

HRESULT CGameInstance::Initialize_ModelMgr(const wstring& strMainPath)
{
	if (nullptr != m_pModelMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pModelMgr = CModelMgr::Create(strMainPath), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Load_Model(const EModelGroupIndex eGroupIndex, const wstring& strFileName)
{
	if (nullptr == m_pModelMgr)
		return E_FAIL;

	return m_pModelMgr->Load_Model(eGroupIndex, strFileName);
}

const FModelData* const CGameInstance::Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_ModelData(eGroupIndex, strModelKey);
}

const FMeshData* const CGameInstance::Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey, const _uint iRangeIndex)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_MeshData(eGroupIndex, strModelKey, strMeshKey, iRangeIndex);
}

const FMeshData* const CGameInstance::Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_MeshData(eGroupIndex, strModelKey, iIndex);
}

const CMeshGroup* const CGameInstance::Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_MeshGroup(eGroupIndex, strModelKey);
}

CBoneGroup* CGameInstance::Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Clone_BoneGroup(eGroupIndex, strModelKey);
}

CBoneGroup* CGameInstance::Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_BoneGroup(eGroupIndex, strModelKey);
}

CBoneAnimGroup* CGameInstance::Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_AnimGroup(eGroupIndex, strModelKey);
}

const FMaterialData* const CGameInstance::Find_MaterialData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Find_MaterialData(eGroupIndex, strModelKey, iIndex);
}

HRESULT CGameInstance::Initialize_ShaderMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	if (nullptr != m_pShaderMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pShaderMgr = CShaderMgr::Create(tDevice, strMainPath), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey)
{
	if (nullptr == m_pShaderMgr)
		return E_POINTER;

	return m_pShaderMgr->Load_Shader(strFileName, eType, strKey);
}


const ComPtr<ID3DBlob> CGameInstance::Get_ShaderByte(const EShaderType eType, const wstring& strKey)
{
	if (nullptr == m_pShaderMgr)
		return nullptr;

	return m_pShaderMgr->Get_ShaderByte(eType, strKey);
}



HRESULT CGameInstance::Load_Effect(const wstring& strFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements
	, D3D_SHADER_MACRO* pShaderMacro)
{
	if (nullptr == m_pShaderMgr)
		return E_FAIL;

	return m_pShaderMgr->Load_Effect(strFileName, pElements, iNumElements, pShaderMacro);
}

ID3DX11Effect* CGameInstance::Find_Effect(const wstring& strEffectFileName) const
{
	if (nullptr == m_pShaderMgr)
		return nullptr;

	return m_pShaderMgr->Find_Effect(strEffectFileName);
}

FEffectData* CGameInstance::Find_EffectData(const wstring& strEffectFileName) const
{
	if (nullptr == m_pShaderMgr)
		return nullptr;

	return m_pShaderMgr->Find_EffectData(strEffectFileName);
}

CShaderMgr* CGameInstance::Get_ShaderMgr()
{
	return m_pShaderMgr;
}
