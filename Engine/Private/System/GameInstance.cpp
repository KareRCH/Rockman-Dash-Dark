#include "System/GameInstance.h"

#include "System/GraphicDev.h"
#include "System/InputDev.h"
#include "System/KeyMgr.h"
#include "System/PhysicsMgr.h"
#include "System/SoundMgr.h"
#include "System/FrameMgr.h"
#include "System/TimerMgr.h"
#include "System/FontMgr.h"
#include "System/ObjectMgr.h"
#include "System/BlackBoardMgr.h"
#include "System/TextureMgr.h"
#include "System/ProtoMgr.h"
#include "System/RenderMgr.h"
#include "BaseClass/GameObject.h"
#include "System/ModelMgr.h"
#include "System/ParticleMgr.h"


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
	Safe_Release(m_pKeyMgr);
	Safe_Release(m_pPhysicsMgr);
	Safe_Release(m_pSoundMgr);
	Safe_Release(m_pFrameMgr);
	Safe_Release(m_pTimerMgr);
	Safe_Release(m_pManagement);
	Safe_Release(m_pBlackBoardMgr);
	
	Safe_Release(m_pProtoMgr);
	Safe_Release(m_pRenderMgr);

	Safe_Release(m_pParticleMgr);
	Safe_Release(m_pShaderMgr);
	Safe_Release(m_pModelMgr);
	Safe_Release(m_pTextureMgr);
	Safe_Release(m_pFontMgr);
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

const _matrix* CGameInstance::Get_GraphicDev_ProjectionMatrix()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return &m_pGraphicDev->GetProjectionMatrix();
}

const _matrix* CGameInstance::Get_GraphicDev_WorldMatrix()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return &m_pGraphicDev->GetWorldMatrix();
}

const _matrix* CGameInstance::Get_GraphicDev_OrthoMatrix()
{
	if (nullptr == m_pGraphicDev)
		return nullptr;

	return &m_pGraphicDev->GetOrthoMatrix();
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



#pragma region 매니지먼트

HRESULT CGameInstance::Initialize_ObjectMgr(const DX11DEVICE_T tDevice, const EMANAGE_SCENE eManageSceneType)
{
	if (nullptr != m_pManagement)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pManagement = CObjectMgr::Create(tDevice, eManageSceneType), E_FAIL);

	return S_OK;
}

void CGameInstance::Priority_Tick_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pManagement)
		return;

	m_pManagement->Priority_Tick(fTimeDelta);
}

_int CGameInstance::Tick_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pManagement)
		return -1;

	m_pManagement->Tick(fTimeDelta);

	return 0;
}

void CGameInstance::Late_Tick_Scene(const _float& fTimeDelta)
{
	if (nullptr == m_pManagement)
		return;

	m_pManagement->Late_Tick(fTimeDelta);
}

void CGameInstance::Render_Scene()
{
	if (nullptr == m_pManagement)
		return;

	m_pManagement->Render();
}

HRESULT CGameInstance::Set_Scene(CLevel* pScene)
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

HRESULT CGameInstance::Initialize_TextureMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	if (nullptr != m_pTextureMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pTextureMgr = CTextureMgr::Create(tDevice, strMainPath), E_FAIL);

	return S_OK;
}

HRESULT CGameInstance::Load_Texture(const wstring& strFileName, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent)
{
	if (nullptr == m_pTextureMgr)
		return E_FAIL;

	return m_pTextureMgr->Load_Texture(strFileName, strGroupKey, strTextureKey, bPermanent);
}

ID3D11ShaderResourceView* CGameInstance::Get_Texture(const wstring& strGroupKey, const wstring& strTextureKey)
{
	if (nullptr == m_pTextureMgr)
		return nullptr;

	return m_pTextureMgr->Get_Texture(strGroupKey, strTextureKey);
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

HRESULT CGameInstance::Initialize_ModelMgr(const string& strMainPath)
{
	if (nullptr != m_pModelMgr)
		return E_FAIL;

	NULL_CHECK_RETURN(m_pModelMgr = CModelMgr::Create(strMainPath), E_FAIL);

	return S_OK;
}

void CGameInstance::Load_Model(const string& strFileName, const wstring& strGroupKey)
{
	if (nullptr == m_pModelMgr)
		return;

	m_pModelMgr->Load_Model(strFileName, strGroupKey);
}

const FMeshData* const CGameInstance::Get_Mesh(const wstring& strGroupKey, const wstring& strMeshKey)
{
	if (nullptr == m_pModelMgr)
		return nullptr;

	return m_pModelMgr->Get_Mesh(strGroupKey, strMeshKey);
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


const ComPtr<ID3DBlob>& CGameInstance::Get_ShaderByte(const EShaderType eType, const wstring& strKey)
{
	if (nullptr == m_pShaderMgr)
		return nullptr;

	return m_pShaderMgr->Get_ShaderByte(eType, strKey);
}

