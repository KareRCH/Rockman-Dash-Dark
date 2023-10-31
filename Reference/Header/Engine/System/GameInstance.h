#pragma once


#include "Base.h"

#include "Physics/PhysicsPrecision.h"

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

class CScene;
class FCollisionPrimitive;

/// <summary>
/// Ŭ���̾�Ʈ���� ������ ����� ����ϱ� ���� �ݵ�� ���ľ��ϴ� ��ü
/// ������ Export_Engine�� ��ü�Ͽ� ���� ���̴�.
/// 
/// DX11�� ���� �ʿ��� ��ü�� �����ϰ� ������ �ϴ� �۾��� �����Ѵ�.
/// ���� ������ ���� �����ϰ� �ʿ信 ���� ������Ʈ ����
/// ���� ������ ��ü���� �����ϰ� ������Ʈ�ϰ� �����Ѵ�.
/// ���� ������ ���� 
/// </summary>
class ENGINE_DLL CGameInstance final : public CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CGameInstance)

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize();

private:
	virtual void Free() override;

public:		// �׷��� ����̽�
	HRESULT					Initialize_GraphicDev(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
													_float fScreenDepth, _float fScreenNear);
	HRESULT					Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT					Clear_DepthStencil_View();
	HRESULT					Present();
	ID3D11Device*			Get_GraphicDev();
	ID3D11DeviceContext*	Get_GraphicContext();
	


public:		// ��ǲ ����̽�
	HRESULT Initialize_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Tick_InputDev();
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	

public:		// Ű �Ŵ���
	HRESULT Initialize_KeyMgr();
	void	Tick_KeyMgr();
	void	LateTick_KeyMgr();

public:		// ������ �Ŵ���
	HRESULT				Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);
	inline void			StartFrame_PhysicsMgr();
	inline _int			Tick_PhysicsMgr(const Real& fTimeDelta);
	inline void			Pause_PhysicsSimulation(const _uint iWorldID);
	inline void			Play_PhysicsSimulation(const _uint iWorldID);
	inline void			Add_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
	inline void			Delete_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);

public:		// ���� �Ŵ���
	HRESULT Initialize_SoundMgr();

public:		// ��Ʈ �Ŵ���
	HRESULT Initialize_FontMgr();
	HRESULT Create_Font(ID3D11Device* pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2* pPos, D3DCOLOR Color);

public:		// ������ �Ŵ���
	HRESULT Initialize_FrameMgr();
	HRESULT Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
	_bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	const _float Get_FrameRate(const _tchar* pFrameTag);

public:		// Ÿ�̸� �Ŵ���
	HRESULT Initialize_TimerMgr();
	HRESULT Create_Timer(const _tchar* pTimerTag);
	_float	Get_TimerDelta(const _tchar* pTimerTag);
	void	Tick_Timer(const _tchar* pTimeTag);

public:		// �Ŵ�����Ʈ
	HRESULT Initialize_Management(const EMANAGE_SCENE eManageSceneType);
	_int	Tick_Scene(const _float& fTimeDelta);
	void	LateTick_Scene();
	void	Render_Scene(ID3D11DeviceContext* const pDeviceContext);
	HRESULT	Set_Scene(CScene* pScene);

public:		// ������ �Ŵ���
	HRESULT Initialize_BlackBoardMgr();

public:		// �ؽ�ó �Ŵ���
	HRESULT Initialize_TextureMgr(ID3D11Device* const pGraphicDev);

public:		// ������ �Ŵ���
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

inline _uint Release_GameInstance()
{
	return CGameInstance::GetInstance()->DestroyInstance();
}

END