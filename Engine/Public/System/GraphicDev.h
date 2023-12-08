#pragma once

#include "Base.h"
#include "GraphicDev_Include.h"

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
	HRESULT		Create_RenderTargets();
	// 깊이 스텐실 버퍼
	HRESULT		Resize_DepthStencil();

public:
	void		TurnOn_ZBuffer();
	void		TurnOff_ZBuffer();
	void		TurnOn_Cull();
	void		TurnOff_Cull();

public:
	HRESULT		Copy_BackBufferToSRV_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, _uint iViewportIndex);

public:
	GETSET_1(ComPtr<ID3D11Device>, m_pDevice, Device, GET_C_REF)
	GETSET_1(ComPtr<ID3D11DeviceContext>, m_pDeviceContext, DeviceContext, GET_C_REF)

private:
	_bool	m_bVsync_Enabled = false;
	_int	m_iVideoCardMemory = 0;
	_char	m_cVideoCardDescription[128] = {0};

	ComPtr<ID3D11Device>			m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext>		m_pDeviceContext = { nullptr };
	ComPtr<IDXGISwapChain>			m_pSwapChain = { nullptr };
	

private:	// 렌더 타겟
	ComPtr<ID3D11Texture2D>			m_pTexture[MaxRenderTarget] = { nullptr };
	ComPtr<ID3D11RenderTargetView>	m_pRTV[MaxRenderTarget] = { nullptr };		// 0번은 항상 스왑체인 RTV
	_uint	m_iNumRenderTargets = 0U;

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

private:
	ComPtr<ID3D11Debug> m_pDebug = { nullptr };
#endif

private:
	_uint	m_iScreenWidth = 1280U;
	_uint	m_iScreenHeight = 720U;

};

END