#pragma once

#include "Base.h"
#include "GraphicDev_Include.h"

#include <dxgidebug.h>

BEGIN(Engine)

struct FDEVICE_INIT
{
	HWND	hWnd;
	_bool	bVSync;
	_bool	bFullScreen;
	_uint	iScreenWidth;
	_uint	iScreenHeight;
	_float	fScreenNear;
	_float	fScreenDepth;
	_uint	iResolutionX;
	_uint	iResolutionY;
	_uint	iRenderTargetCount;
	wstring strMainPath;
};

/// <summary>
/// 그래픽장치를 초기화하는 클래스
/// </summary>
class CGraphicDev : public CBase
{
	DERIVED_CLASS(CBase, CGraphicDev)
public:
	explicit CGraphicDev();
	explicit CGraphicDev(const CGraphicDev& rhs) = delete;
	virtual ~CGraphicDev() = default;

public:
	HRESULT		Initialize(const FDEVICE_INIT& tInit);
	HRESULT		Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT		Clear_DepthStencil_View();
	HRESULT		Present();

public:
	static CGraphicDev* Create(const FDEVICE_INIT& tInit);

private:
	virtual void Free();

private:
	HRESULT		Ready_SwapChain(const FDEVICE_INIT& tInit);
	HRESULT		Ready_BackBufferRenderTargetView(const FDEVICE_INIT& tInit);
	HRESULT		Ready_DepthStencilRenderTargetView(const FDEVICE_INIT& tInit);
	HRESULT		Ready_RasterizeState(const FDEVICE_INIT& tInit);
	HRESULT		Ready_Viewport(const FDEVICE_INIT& tInit);

public:
	HRESULT		Resize_SwapChain(_uint iWidth, _uint iHeight);
	// 렌더 타겟 조합에 추가하기
	HRESULT		Regist_RenderTarget(_uint iRenderTargetIndex);
	// 장치에 렌더타겟 바인딩
	void		Bind_RenderTargetsOnDevice();

private:
	// 렌더 타겟 파괴
	void		CleanUp_RenderTargets();
	// 렌더 타겟 생성, 재생성용
	HRESULT		Create_RenderTargets(_bool bIsInit);
	// 깊이 스텐실 버퍼
	HRESULT		Resize_DepthStencil();

public:
	void		TurnOn_ZBuffer();
	void		TurnOff_ZBuffer();
	void		TurnOn_Cull();
	void		TurnOff_Cull();



public:
	GETSET_1(ComPtr<ID3D11Device>, m_pDevice, Device, GET_C_REF)
	GETSET_1(ComPtr<ID3D11DeviceContext>, m_pDeviceContext, DeviceContext, GET_C_REF)

private:
	_bool	m_bVsync_Enabled = false;
	_int	m_iVideoCardMemory = 0;
	_char	m_cVideoCardDescription[128] = {0};

	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };
	ComPtr<IDXGIDevice>				m_pDxgiDevice = { nullptr };
	ComPtr<IDXGISwapChain>			m_pSwapChain = { nullptr };


public:
	// 뷰포트 사이즈로 렌더타겟에 기록된 정보들을 텍스처 리소스로 복사해오는 함수. 툴에서 쓰임.
	HRESULT		Copy_RenderTargetViewToTexture_ByViewport(ComPtr<ID3D11Texture2D>& pTexture, _uint iRenderTargetIndex, _uint iViewportIndex);
	// 뷰포트 사이즈로 백버퍼에 기록된 픽셀들을 셰이덜 리소스로 복사해오는 함수
	HRESULT		Copy_BackBufferToSRV_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, _uint iViewportIndex);
	HRESULT		Copy_BackBufferToSRV_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, const D3D11_VIEWPORT Viewport);
	// 렌더타겟 데이터 불러와 설정을 변경하는데 쓸 수 있다.
	const D3D11_TEXTURE2D_DESC* Get_RTV_Desc(_uint iIndex) const;
	// 렌더타겟 설정변경, 렌더타겟도 같이 바뀐다.
	void						Set_RTV_Desc(_uint iIndex, D3D11_TEXTURE2D_DESC& Desc);
	ID3D11RenderTargetView*		Get_BackBufferRTV() const { return m_pRTV[0].Get(); }
	ID3D11DepthStencilView*		Get_DSV() const { return m_pDepthStencilView.Get(); }
	

private:	// 렌더 타겟
	D3D11_RENDER_TARGET_VIEW_DESC	m_RTV_Descs[MaxRenderTarget] = {};			// 렌더 타겟 설정들을 세팅한다.
	D3D11_TEXTURE2D_DESC			m_RTVTexture_Descs[MaxRenderTarget] = {};	// 렌더 타겟의 텍스처 설정을 세팅할 수 있다.
	ComPtr<ID3D11RenderTargetView>	m_pRTV[MaxRenderTarget] = { nullptr };		// 0번은 항상 스왑체인 RTV
	_uint							m_iNumRenderTargets = 0U;					// 렌더타겟 수

	using vector_RTV = vector<ID3D11RenderTargetView*>;
	vector_RTV						m_vecRTV;									// 실제 할당되는 렌더타깃의 벡터

private:	// 스텐실
	ComPtr<ID3D11Texture2D>			m_pDepthStencilBuffer = nullptr;			// 스텐실 뷰 생성용 버퍼
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;				// 깊이 스텐실 뷰

	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;				// 일반 스텐실 상태
	ComPtr<ID3D11DepthStencilState>	m_pDepthDisabledStencilState = nullptr;		// Z버퍼 Off한 스텐실 상태

	ComPtr<ID3D11RasterizerState>	m_pRasterState = { nullptr };				// 일반 래스터라이즈 상태
	ComPtr<ID3D11RasterizerState>	m_pRasterCullNoneState = { nullptr };		// 컬을 하지 않는 래스터라이즈


public:		// 시스템 렌더를 위해 쓰이는 뷰포트 세팅이다.
	void Add_Viewport(D3D11_VIEWPORT Viewport);
	void Set_Viewport(_uint iIndex, D3D11_VIEWPORT Viewport);
	D3D11_VIEWPORT* Get_ViewportPtr(_uint iIndex);
	// 뷰포트를 세팅한다.
	HRESULT Bind_Viewport();
	HRESULT Bind_Viewport(_uint iIndex);

private:	// 시스템 렌더링용 뷰포트
	vector<D3D11_VIEWPORT>		m_vecViewports;
	_uint	m_iNumViewports = 0;


#ifdef _DEBUG
public:
	GETSET_1(ID3D11Debug*, m_pDebug.Get(), Debug, GET__C)
	GETSET_1(IDXGIDebug*, m_pDxDebug.Get(), DxDebug, GET__C)

private:
	ComPtr<ID3D11Debug> m_pDebug = { nullptr };
	ComPtr<IDXGIDebug>	m_pDxDebug = { nullptr };
#endif

public:
	void			Set_ScreenSize(_uint iX, _uint iY);
	void			Set_ScreenWidth(_uint iX);
	void			Set_ScreenHeight(_uint iY);

private:	// 창 크기, 해상도에 영향을 주지 않습니다.
	_uint			m_iScreenWidth = g_iWindowSizeX;
	_uint			m_iScreenHeight = g_iWindowSizeY;
	_float			m_iScreenRatio = 1.f;

public:		// 해상도 세팅 관련
	void			Set_Resolution(_uint iX, _uint iY);
	void			Set_ResolutionX_MaintainRatio(_uint iX);
	void			Set_ResolutionY_MaintainRatio(_uint iY);
	const _uint&	Get_ResolutionX() const { return m_viResolutionX; }
	const _uint&	Get_ResolutionY() const { return m_viResolutionY; }
	const _float&	Get_ResolutionRatio() const { return m_fResolutionRatio; }

private:	// 해상도는 실제 게임이 표시되는 값에 영향을 줍니다. 백버퍼의 크기보다 작거나 클 수 있습니다.
	_uint			m_viResolutionX = g_iWindowSizeX;
	_uint			m_viResolutionY = g_iWindowSizeY;
	_float			m_fResolutionRatio = 1.f;

public:
	const _float&	Get_ResolutionByScreen_RatioX() { return m_vResolutionByScreen_Ratio.x; }
	const _float&	Get_ResolutionByScreen_RatioY() { return m_vResolutionByScreen_Ratio.y; }

private:			// 해상도 : 화면비, 실제 표시되는 영역을 지정하기 위해 사용됩니다.
	void			Apply_ResolutionByScreen_Ratio();

private:
	_float2			m_vResolutionByScreen_Ratio = {};

};

END