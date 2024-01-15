#pragma once


#include "Base.h"

#include "Physics/PhysicsPrecision.h"
#include "System/ShaderMgr.h"
#include "System/Define/ModelMgr_Define.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)

struct FDEVICE_INIT;
enum class EMANAGE_SCENE : _uint;
enum class ERenderGroup : _uint;
class FModelData;
class FMeshData;
class CMeshGroup;
class FMaterialData;
class FContact;
class CColliderComponent;

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
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd, FDEVICE_INIT tDeviceInit);
	void	Clear(const wstring& strLevelTag);

private:
	virtual void Free() override;

public:
	void Release_Managers();

public:
	wstring Get_MainPath();


#pragma region 그래픽 디바이스
public:		// 그래픽 디바이스
	HRESULT							Initialize_GraphicDev(const FDEVICE_INIT& tInit);
	HRESULT							Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT							Clear_DepthStencil_View();
	HRESULT							Present();

	ComPtr<ID3D11Device>			Get_GraphicDev();
	ComPtr<ID3D11DeviceContext>		Get_GraphicContext();
	ID3D11RenderTargetView*			Get_BackBufferRTV() const;
	ID3D11DepthStencilView*			Get_DSV() const;
			
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

	const D3D11_TEXTURE2D_DESC*		Get_RTV_Desc(_uint iIndex);
	void							Set_RTV_Desc(_uint iIndex, D3D11_TEXTURE2D_DESC& Desc);

	HRESULT							Copy_RenderTargetViewToTexture_ByViewport(ComPtr<ID3D11Texture2D>& pTexture, _uint iRenderTargetIndex, _uint iViewportIndex);
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
	_bool		IsKey_Pressing(const int& iKey);
	_bool		IsKey_Pressed(const int& iKey);
	_bool		IsKey_Released(const int& iKey);
	// 마우스 입력
	_bool		IsMouse_Pressing(const MOUSEKEYSTATE& iMouse);
	_bool		IsMouse_Pressed(const MOUSEKEYSTATE& iMouse);
	_bool		IsMouse_Released(const MOUSEKEYSTATE& iMouse);


#pragma endregion




#pragma region 피직스 매니저
public:		// 피직스 매니저
	HRESULT				Initialize_PhysicsMgr(_uint iPhysicsWorldCount = 1);
	void			StartFrame_PhysicsMgr();
	_int			Tick_PhysicsMgr(const _float& fTimeDelta);
	void			Pause_PhysicsSimulation(const _uint iWorldID);
	void			Play_PhysicsSimulation(const _uint iWorldID);
	void			Add_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);
	void			Delete_ColliderToPhysicsWorld(const _uint iWorldID, FCollisionPrimitive* pCollider);

	list<pair<class CGameObject*, FContact>> IntersectTests_Collider_GetGameObject(const _uint iWorldID, const _float3 vPos, CColliderComponent* pSrc, _ulong iMask);
	list<pair<class CGameObject*, FContact>> IntersectTests_Sphere_GetGameObject(const _uint iWorldID, _float3 vPos, _float fRadius, _ulong iMask);
	list<pair<class CGameObject*, FContact>> IntersectTests_Box_GetGameObject(const _uint iWorldID, _float3 vPos, _float3 vHalfSize, _ulong iMask);
	list<pair<class CGameObject*, FContact>> IntersectTests_Capsule_GetGameObject(const _uint iWorldID, _float3 vPos, _float3 vNormal, _float fRadius, _ulong iMask);
	list<pair<class CGameObject*, FContact>> IntersectTests_Line_GetGameObject(const _uint iWorldID, _float3 vStart, _float3 vEnd, _ulong iMask);
	list<pair<class CGameObject*, FContact>> IntersectTests_Ray_GetGameObject(const _uint iWorldID, const _float3 vPos, const _float3 vNormal, _ulong iMask);
#pragma endregion




#pragma region 사운드 매니저
public:		// 사운드 매니저
	HRESULT Initialize_SoundMgr(const string& strMainPath);
	void	Play_Sound(const wstring& strGroupKey, const wstring& strSoundKey, CHANNELID eID, _float fVolume);
	void	Play_BGM(const wstring& strGroupKey, const wstring& strSoundKey, _float fVolume);
	void	Stop_Sound(CHANNELID eID);
	void	Stop_SoundAll();
	void	Set_ChannelVolume(CHANNELID eID, _float fVolume);
#pragma endregion



#pragma region 폰트 매니저
public:		// 폰트 매니저
	HRESULT Initialize_FontMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT Add_Font(const wstring& strFontTag, const wstring& strFontFilePath);
	HRESULT Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, 
						_fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fScale = 1.f, 
						_float2 vOrigin = _float2(0.f, 0.f), _float fRotation = 0.f);
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
	void Tick_PipelineMgr();

private:
	class CPipelineMgr* Get_PipelineMgr();
#pragma endregion




#pragma region 레벨 매니저
public:
	HRESULT Initialize_LevelMgr();
	void	Tick_LevelMgr(const _float& fTimeDelta);
	HRESULT Open_Level(_uint iCurrentLevelIndex, class CLevel* pNewLevel);
#pragma endregion



#pragma region 오브젝트 매니저
public:		// 매니지먼트
	HRESULT				Initialize_ObjectMgr();
	void				Priority_Tick_Object(const _float& fTimeDelta);
	_int				Tick_Object(const _float& fTimeDelta);
	void				Late_Tick_Object(const _float& fTimeDelta);

	HRESULT				Add_PrototypeObject(const wstring& strTag, const wstring& strPrototypeKey, class CGameObject* pPrototype);
	HRESULT				Add_CloneObject(const wstring& strPrototypeKey, void* pArg = nullptr);
	class CGameObject*	Find_PrototypeObject(const wstring& strPrototypeKey);
	void				Clear_PrototypeObejcts(const wstring& strContainTag);

	void				Set_LevelTag(const wstring& strLevelTag);
	HRESULT				Add_GameObject(class CGameObject* pObj);
	HRESULT				Add_GameObject(const wstring& strLevelTag, class CGameObject* pObj);
	class CGameObject*	Find_GameObjectByID(_uint iFindID);
	class CGameObject*	Find_GameObjectByIndex(_uint iIndex);
	class CGameObject*	Find_GameObjectByName(const wstring& strName);
	vector<class CGameObject*> Get_AllGameObjectFromLevel(const wstring& strLevelTag);
	void				Pause_ObjectsByLevelTag(const wstring& strLevelTag);
	void				Resume_ObjectsByLevelTag(const wstring& strLevelTag);
	void				Clear_GameObject(const wstring& strLayerTag);
#pragma endregion



#pragma region 컴포넌트 매니저
public:		// 컴포넌트 매니저
	HRESULT				Initialize_ComponentMgr();
	HRESULT				Add_PrototypeComp(const wstring& strTag, const wstring& strProtoKey, class CComponent* pPrototype);
	class CComponent*	Clone_PrototypeComp(const wstring& strProtoKey, void* pArg = nullptr);
	class CComponent*	Reference_PrototypeComp(const wstring& strProtoKey);
	void				Clear_PrototypeComps(const wstring& strContainTag);
#pragma endregion



#pragma region 클라우드 스테이션 매니저
	friend class CCloudStationComp;
public:		// 클라우드 스테이션 매니저
	HRESULT			Initialize_CloudStationMgr();
	void			Update_CloudStationMgr();
	HRESULT			Add_CloudStation(const wstring& strBoardName, class CCloudStation* pCloudStation);
	CCloudStation*	Get_CloudStation(const wstring& strBoardName);

private:
	class CCloudStationMgr*		Get_CloudStationMgr();
#pragma endregion



#pragma region 텍스처 매니저
	friend class CTextureComponent;
public:		// 텍스처 매니저
	HRESULT						Initialize_TextureMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT						IsReady_TextureMgr();
	HRESULT						Load_Texture(const wstring& strFileName, const _uint iNumTextures, const _bool bPermanent, const _bool bUseMainPath = true);
	ID3D11ShaderResourceView*	Find_SRV(const wstring& strTextureKey, const _uint iIndex = 0);
	const wstring				Get_TextureMainPath();

private:
	class CTextureMgr*			Get_TextureMgr();
#pragma endregion



#pragma region 렌더 매니저
public:		// 렌더 매니저
	HRESULT			Initialize_RenderMgr(const DX11DEVICE_T tDevice, const _uint iWidth = 1280U, const _uint iHeight = 720U);
	void			Render();
	void			Add_RenderGroup(ERenderGroup eType, class CGameObject* pGameObject);
	void			Clear_RenderGroup();
	void			Add_DebugEvent(FastDelegate0<HRESULT> Event);
	void			Toggle_Deferred();
#pragma endregion

#pragma region 렌더타겟 매니저
public:
	HRESULT	Initialize_RenderTargetMgr(const DX11DEVICE_T tDevice);
	HRESULT	Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT	Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Begin_MRT(const wstring& strMRTTag);
	HRESULT End_MRT();
	HRESULT Bind_RenderTarget_ShaderResource(const wstring& strTargetTag, class CEffectComponent* pEffect, const _char* pConstantName);
#ifdef _DEBUG
	HRESULT Ready_RenderTarget_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug_RTVs(const wstring& strMRTTag, class CEffectComponent* pEffect, class CRectBufferComp* pVIBuffer);
#endif

#pragma endregion

#pragma region 라이트 매니저
public: /* For.Light_Manager */
	HRESULT Initialize_LightMgr();
	HRESULT Add_Light(const TLIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CEffectComponent* pEffect, class CRectBufferComp* pVIBuffer);
#pragma endregion



#pragma region 모델 매니저
public:		// 모델 매니저
	HRESULT					Initialize_ModelMgr(const wstring& strMainPath);
	HRESULT					Load_Model(const EModelGroupIndex eGroupIndex, const wstring& strFileName);
	const FModelData* const Find_ModelData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strMeshKey, const _uint iRangeIndex);
	const FMeshData* const	Find_MeshData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex);
	const CMeshGroup* const Find_MeshGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	class CBoneGroup*		Clone_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	class CBoneGroup*		Find_BoneGroup(const EModelGroupIndex eGroupIndex, const wstring& strGroupKey);
	class CBoneAnimGroup*	Find_AnimGroup(const EModelGroupIndex eGroupIndex, const wstring& strModelKey);
	const FMaterialData* const Find_MaterialData(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const _uint iIndex);

#pragma endregion



#pragma region 셰이더 매니저
	friend class CEffectComponent;
public:		// 셰이더 매니저
	HRESULT							Initialize_ShaderMgr(const DX11DEVICE_T tDevice, const wstring& strMainPath);
	HRESULT							Load_Shader(const wstring& strFileName, const EShaderType eType, const wstring& strKey);
	const ComPtr<ID3DBlob>			Get_ShaderByte(const EShaderType eType, const wstring& strKey);
	template<EShaderType Type>
	inline ComPtr<ShaderType<Type>> Get_ShaderBuffer(const wstring& strKey);
	HRESULT							Load_Effect(const wstring& strFileName, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements, const D3D_SHADER_MACRO* pShaderMacro = nullptr);
	ID3DX11Effect*					Find_Effect(const wstring& strEffectFileName) const;
	class FEffectData*				Find_EffectData(const wstring& strEffectFileName, const D3D_SHADER_MACRO* pShaderMacro = nullptr) const;

private:	// 셰이더 매니저 귀속형 객체 전용
	class CShaderMgr*				Get_ShaderMgr();
#pragma endregion

private:
	wstring m_strMainPath = TEXT("");

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
	class CRenderTargetMgr* m_pRenderTargetMgr = { nullptr };
	class CLightMgr*		m_pLightMgr = { nullptr };
	class CCloudStationMgr*	m_pCloudStationMgr = nullptr;
	
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