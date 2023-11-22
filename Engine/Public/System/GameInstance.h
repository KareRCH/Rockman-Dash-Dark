#pragma once


#include "Base.h"

#include "Physics/PhysicsPrecision.h"
#include "System/ShaderMgr.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

struct FDEVICE_INIT;
enum class EMANAGE_SCENE : _uint;
enum class ERenderGroup : _uint;
class FMeshData;

class CLevel;
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


#pragma region �׷��� ����̽�
public:		// �׷��� ����̽�
	HRESULT							Initialize_GraphicDev(const FDEVICE_INIT& tInit);
	HRESULT							Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT							Clear_DepthStencil_View();
	HRESULT							Present();
	ComPtr<ID3D11Device>			Get_GraphicDev();
	ComPtr<ID3D11DeviceContext>		Get_GraphicContext();
	void							TurnOn_ZBuffer();
	void							TurnOff_ZBuffer();
	void							TurnOn_Cull();
	void							TurnOff_Cull();
	const _matrix* Get_GraphicDev_ProjectionMatrix();
	const _matrix* Get_GraphicDev_WorldMatrix();
	const _matrix* Get_GraphicDev_OrthoMatrix();
#pragma endregion


#pragma region ��ǲ ����̽�
public:		// ��ǲ ����̽�
	HRESULT Initialize_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Tick_InputDev();
	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
#pragma endregion


#pragma region Ű �Ŵ���
public:		// Ű �Ŵ���
	HRESULT Initialize_KeyMgr();
	void	Tick_KeyMgr();
	void	Late_Tick_KeyMgr();
	// Ű �Է�
	inline _bool		IsKey_Pressing(const int& iKey);
	inline _bool		IsKey_Pressed(const int& iKey);
	inline _bool		IsKey_Released(const int& iKey);
	// ���콺 �Է�
	inline _bool		IsMouse_Pressing(const MOUSEKEYSTATE& iMouse);
	inline _bool		IsMouse_Pressed(const MOUSEKEYSTATE& iMouse);
	inline _bool		IsMouse_Released(const MOUSEKEYSTATE& iMouse);

public:		// ������ �Ŵ���
	HRESULT				Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);
	inline void			StartFrame_PhysicsMgr();
	inline _int			Tick_PhysicsMgr(const Real& fTimeDelta);
	inline void			Pause_PhysicsSimulation(const _uint iWorldID);
	inline void			Play_PhysicsSimulation(const _uint iWorldID);
	inline void			Add_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
	inline void			Delete_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
#pragma endregion


#pragma region ���� �Ŵ���
public:		// ���� �Ŵ���
	HRESULT Initialize_SoundMgr();
#pragma endregion


#pragma region ��Ʈ �Ŵ���
public:		// ��Ʈ �Ŵ���
	HRESULT Initialize_FontMgr(const DX11DEVICE_T tDevice);
	HRESULT Create_Font(const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2* pPos, D3DCOLOR Color);
#pragma endregion


#pragma region ������ �Ŵ���
public:		// ������ �Ŵ���
	HRESULT Initialize_FrameMgr();
	HRESULT Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
	_bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	const _float Get_FrameRate(const _tchar* pFrameTag);
#pragma endregion


#pragma region Ÿ�̸� �Ŵ���
public:		// Ÿ�̸� �Ŵ���
	HRESULT Initialize_TimerMgr();
	HRESULT Create_Timer(const _tchar* pTimerTag);
	_float	Get_TimerDelta(const _tchar* pTimerTag);
	void	Tick_Timer(const _tchar* pTimeTag);
#pragma endregion

#pragma region ���� �Ŵ���
public:
	HRESULT Initialize_LevelMgr();
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);

#pragma endregion

#pragma region ������Ʈ �Ŵ���
public:		// �Ŵ�����Ʈ
	HRESULT Initialize_ObjectMgr();
	void	Priority_Tick_Object(const _float& fTimeDelta);
	_int	Tick_Object(const _float& fTimeDelta);
	void	Late_Tick_Object(const _float& fTimeDelta);
#pragma endregion


#pragma region ������Ʈ �Ŵ���
public:		// ������Ʈ �Ŵ���
	HRESULT Initialize_ComponentMgr();
#pragma endregion


#pragma region ������ �Ŵ���
public:		// ������ �Ŵ���
	HRESULT Initialize_BlackBoardMgr();
#pragma endregion


#pragma region �ؽ�ó �Ŵ���
public:		// �ؽ�ó �Ŵ���
	HRESULT Initialize_TextureMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT Load_Texture(const wstring& strFileName, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent);
	ID3D11ShaderResourceView* Get_Texture(const wstring& strGroupKey, const wstring& strTextureKey);
#pragma endregion


#pragma region ���� �Ŵ���
public:		// ���� �Ŵ���
	HRESULT	Initialize_RenderMgr(const DX11DEVICE_T tDevice, const _uint iWidth = 1280U, const _uint iHeight = 720U);
	void	Render();
	void	Add_RenderGroup(ERenderGroup eType, class CGameObject* pGameObject);
	void	Clear_RenderGroup();
	void			Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPersView);
	const _matrix 	Get_PerspectiveViewMatrix(const _uint iCam) const;
	void			Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix	Get_PerspectiveProjMatrix(const _uint iCam) const;
	void			Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrthoView);
	const _matrix	Get_OrthogonalViewMatrix(const _uint iCam) const;
	void			Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matOrthoProj);
	const _matrix	Get_OrthogonalProjMatrix(const _uint iCam) const;
#pragma endregion


#pragma region �� �Ŵ���
public:		// �� �Ŵ���
	HRESULT	Initialize_ModelMgr(const string& strMainPath);
	void	Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strGroupKey);
	const FMeshData* const Get_Mesh(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strMeshKey);
	class FArmatureData* Clone_Armature(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey, const wstring& strArmatureKey);

#pragma endregion


#pragma region ���̴� �Ŵ���
public:		// ���̴� �Ŵ���
	HRESULT Initialize_ShaderMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT	Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey);
	const ComPtr<ID3DBlob>& Get_ShaderByte(const EShaderType eType, const wstring& strKey);
	template<EShaderType Type>
	inline ComPtr<ShaderType<Type>> Get_ShaderBuffer(const wstring& strKey);
#pragma endregion



private:
	class CGraphicDev*		m_pGraphicDev = nullptr;
	class CInputDev*		m_pInputDev = nullptr;
	class CKeyMgr*			m_pKeyMgr = nullptr;
	class CPhysicsMgr*		m_pPhysicsMgr = nullptr;
	class CSoundMgr*		m_pSoundMgr = nullptr;
	class CFontMgr*			m_pFontMgr = nullptr;
	class CFrameMgr*		m_pFrameMgr = nullptr;
	class CTimerMgr*		m_pTimerMgr = nullptr;

	class CLevelMgr*		m_pLevelMgr = nullptr;
	class CObjectMgr*		m_pObjectMgr = nullptr;
	class CComponentMgr*	m_pComponentMgr = nullptr;
	class CRenderMgr*		m_pRenderMgr = nullptr;
	class CBlackBoardMgr*	m_pBlackBoardMgr = nullptr;
	
	class CModelMgr*		m_pModelMgr = nullptr;
	class CTextureMgr*		m_pTextureMgr = nullptr;
	class CShaderMgr*		m_pShaderMgr = nullptr;
	class CParticleMgr*		m_pParticleMgr = nullptr;
};


inline CGameInstance* GameInstance()
{
	return CGameInstance::GetInstance();
}

// �� ª�Խ��
inline CGameInstance* GI()
{
	return CGameInstance::GetInstance();
}

inline _uint Release_GameInstance()
{
	return CGameInstance::GetInstance()->DestroyInstance();
}

template<EShaderType Type>
inline ComPtr<ShaderType<Type>> CGameInstance::Get_ShaderBuffer(const wstring& strKey)
{
	if (nullptr == m_pShaderMgr)
		return nullptr;

	return m_pShaderMgr->Get_ShaderBuffer<Type>(strKey);
}

END