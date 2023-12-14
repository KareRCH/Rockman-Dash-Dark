#pragma once


#include "Base.h"

#include "Physics/PhysicsPrecision.h"
#include "System/ShaderMgr.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

struct FDEVICE_INIT;
enum class EMANAGE_SCENE : _uint;
enum class ERenderGroup : _uint;
class FModelData;
class FMeshData;
class FMeshGroup;

class CLevel;
class FCollisionPrimitive;

/// <summary>
/// 클라이언트에서 엔진의 기능을 사용하기 위해 반드시 거쳐야하는 객체
/// 기존의 Export_Engine을 대체하여 사용될 것이다.
/// 
/// DX11을 통해 필요한 객체를 생성하고 렌더링 하는 작업을 수행한다.
/// 지급 생성한 씬을 보관하고 필요에 따라 업데이트 수정
/// 내가 생성한 객체들을 보관하고 업데이트하고 렌더한다.
/// 내가 생성한 여러 
/// </summary>
class ENGINE_DLL CGameInstance final : public CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CGameInstance)

private:
	explicit CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);

private:
	virtual void Free() override;


#pragma region 그래픽 디바이스
public:		// 그래픽 디바이스
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
	
	HRESULT							Resize_SwapChain(_uint iWidth, _uint iHeight);
	HRESULT							Regist_RenderTarget(_uint iRenderTargetIndex);
	void							Bind_RenderTargetsOnDevice();

	void							Add_SystemViewport(D3D11_VIEWPORT Viewport);
	void							Set_SystemViewport(_uint iIndex, D3D11_VIEWPORT Viewport);
	D3D11_VIEWPORT*					Get_SystemViewportPtr(_uint iIndex);
	HRESULT							Bind_SystemViewport();
	HRESULT							Bind_SystemViewport(_uint iIndex);

	HRESULT							Copy_BackBufferToTexture_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, _uint iViewportIndex);
	HRESULT							Copy_BackBufferToTexture_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, const D3D11_VIEWPORT Viewport);
	const _float					ResolutionRatio();
	const _uint						ResolutionX();
	const _uint						ResolutionY();
	void							Set_Resolution(_uint iX, _uint iY);
	void							Set_ResolutionX_MaintainRatio(_uint iX);
	void							Set_ResolutionY_MaintainRatio(_uint iY);
	const _float					ResolutionByScreen_RatioX();
	const _float					ResolutionByScreen_RatioY();

#pragma endregion



#pragma region 인풋 디바이스
public:			// 인풋 디바이스
	HRESULT		Initialize_InputDev(HINSTANCE hInst, HWND hWnd);
	void		Tick_InputDev();
	void		Late_Tick_InputDev();
	_byte		Get_DIKeyState(_ubyte byKeyID);
	_byte		Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long		Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	void		Toggle_LockMouseCenter();
#pragma endregion



#pragma region 키 매니저
public:					// 키 매니저
	HRESULT				Initialize_KeyMgr();
	void				Tick_KeyMgr();
	void				Late_Tick_KeyMgr();
	// 키 입력
	inline _bool		IsKey_Pressing(const int& iKey);
	inline _bool		IsKey_Pressed(const int& iKey);
	inline _bool		IsKey_Released(const int& iKey);
	// 마우스 입력
	inline _bool		IsMouse_Pressing(const MOUSEKEYSTATE& iMouse);
	inline _bool		IsMouse_Pressed(const MOUSEKEYSTATE& iMouse);
	inline _bool		IsMouse_Released(const MOUSEKEYSTATE& iMouse);

public:		// 피직스 매니저
	HRESULT				Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);
	inline void			StartFrame_PhysicsMgr();
	inline _int			Tick_PhysicsMgr(const Real& fTimeDelta);
	inline void			Pause_PhysicsSimulation(const _uint iWorldID);
	inline void			Play_PhysicsSimulation(const _uint iWorldID);
	inline void			Add_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
	inline void			Delete_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
#pragma endregion



#pragma region 사운드 매니저
public:		// 사운드 매니저
	HRESULT Initialize_SoundMgr();
#pragma endregion



#pragma region 폰트 매니저
public:		// 폰트 매니저
	HRESULT Initialize_FontMgr(const DX11DEVICE_T tDevice);
	HRESULT Create_Font(const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void	Render_Font(const _tchar* pFontTag, const _tchar* pString, const _float2* pPos, D3DCOLOR Color);
#pragma endregion



#pragma region 프레임 매니저
public:		// 프레임 매니저
	HRESULT Initialize_FrameMgr();
	HRESULT Create_Frame(const _tchar* pFrameTag, const _float& fCallLimit);
	_bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
	const _float Get_FrameRate(const _tchar* pFrameTag);
#pragma endregion



#pragma region 타이머 매니저
public:		// 타이머 매니저
	HRESULT Initialize_TimerMgr();
	HRESULT Create_Timer(const _tchar* pTimerTag);
	_float	Get_TimerDelta(const _tchar* pTimerTag);
	void	Tick_Timer(const _tchar* pTimeTag);
#pragma endregion





#pragma region 파이프라인 매니저
	friend class CPipelineComp;
public:		// 타이머 매니저
	HRESULT Initialize_PipelineMgr();

private:
	class CPipelineMgr* Get_PipelineMgr();
#pragma endregion




#pragma region 레벨 매니저
public:
	HRESULT Initialize_LevelMgr();
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);
#pragma endregion



#pragma region 오브젝트 매니저
public:		// 매니지먼트
	HRESULT				Initialize_ObjectMgr();
	void				Priority_Tick_Object(const _float& fTimeDelta);
	_int				Tick_Object(const _float& fTimeDelta);
	void				Late_Tick_Object(const _float& fTimeDelta);

	HRESULT				Add_PrototypeObject(const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT				Add_CloneObject(const wstring& strPrototypeKey, void* pArg = nullptr);
	class CGameObject*	Find_PrototypeObject(const wstring& strPrototypeKey);
	void				Clear_PrototypeObejcts(const wstring& strContainTag);

	HRESULT				Add_GameObject(class CGameObject* pObj);
	class CGameObject*	Find_GameObject(_uint iFindID);
	void				Clear_GameObject(const wstring& strLayerTag);
#pragma endregion



#pragma region 컴포넌트 매니저
public:		// 컴포넌트 매니저
	HRESULT				Initialize_ComponentMgr();
	HRESULT				Add_PrototypeComp(const wstring& strProtoKey, class CComponent* pPrototype);
	class CComponent*	Clone_PrototypeComp(const wstring& strProtoKey, void* pArg = nullptr);
	class CComponent*	Reference_PrototypeComp(const wstring& strProtoKey);
	void				Clear_PrototypeComps(const wstring& strContainTag);
#pragma endregion



#pragma region 블랙보드 매니저
public:		// 블랙보드 매니저
	HRESULT Initialize_CloudStationMgr();
#pragma endregion



#pragma region 텍스처 매니저
	friend class CTextureComponent;
public:		// 텍스처 매니저
	HRESULT						Initialize_TextureMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT						IsReady_TextureMgr();
	HRESULT						Load_Texture(const wstring& strFileName, const _bool bPermanent);
	ID3D11Texture2D*			Find_Texture2D(const wstring& strTextureKey);
	ID3D11ShaderResourceView*	Find_SRV(const wstring& strTextureKey);
	const wstring				Get_TextureMainPath();

private:
	class CTextureMgr* Get_TextureMgr();
#pragma endregion



#pragma region 렌더 매니저
public:		// 렌더 매니저
	HRESULT			Initialize_RenderMgr(const DX11DEVICE_T tDevice, const _uint iWidth = 1280U, const _uint iHeight = 720U);
	void			Render();
	void			Add_RenderGroup(ERenderGroup eType, class CGameObject* pGameObject);
	void			Clear_RenderGroup();
	void			Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPersView);
	const _matrix 	Get_PerspectiveViewMatrix(const _uint iCam) const;
	void			Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj);
	const _matrix	Get_PerspectiveProjMatrix(const _uint iCam) const;
	void			Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrthoView);
	const _matrix	Get_OrthogonalViewMatrix(const _uint iCam) const;
	void			Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matOrthoProj);
	const _matrix	Get_OrthogonalProjMatrix(const _uint iCam) const;
#pragma endregion



#pragma region 모델 매니저
public:		// 모델 매니저
	HRESULT					Initialize_ModelMgr(const string& strMainPath);
	void					Load_Model(const EModelGroupIndex eGroupIndex, const string& strFileName, const wstring& strGroupKey);
	const FModelData* const Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey);
	const FMeshGroup* const Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	class FBoneGroup*		Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	class FBoneGroup*		Find_Skeletal(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
#pragma endregion



#pragma region 셰이더 매니저
	friend class CEffectComponent;
public:		// 셰이더 매니저
	HRESULT							Initialize_ShaderMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT							Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey);
	const ComPtr<ID3DBlob>			Get_ShaderByte(const EShaderType eType, const wstring& strKey);
	template<EShaderType Type>
	inline ComPtr<ShaderType<Type>> Get_ShaderBuffer(const wstring& strKey);
	HRESULT							Load_Effect(const wstring& strFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	ID3DX11Effect*					Find_Effect(const wstring& strEffectFileName) const;
	class FEffectData*				Find_EffectData(const wstring& strEffectFileName) const;

private:
	class CShaderMgr*				Get_ShaderMgr();
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
	class CPipelineMgr*		m_pPipelineMgr = nullptr;

	class CLevelMgr*		m_pLevelMgr = nullptr;
	class CObjectMgr*		m_pObjectMgr = nullptr;
	class CComponentMgr*	m_pComponentMgr = nullptr;
	class CRenderMgr*		m_pRenderMgr = nullptr;
	class CCloudStationMgr*	m_pBlackBoardMgr = nullptr;
	
	class CModelMgr*		m_pModelMgr = nullptr;
	class CTextureMgr*		m_pTextureMgr = nullptr;
	class CShaderMgr*		m_pShaderMgr = nullptr;
	class CParticleMgr*		m_pParticleMgr = nullptr;
};


inline CGameInstance* GameInstance()
{
	return CGameInstance::GetInstance();
}

// 더 짧게써라
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