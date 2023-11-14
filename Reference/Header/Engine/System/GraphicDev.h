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
	void TurnOn_ZBuffer();
	void TurnOff_ZBuffer();
	void TurnOn_Cull();
	void TurnOff_Cull();

public:
	const _matrix& GetProjectionMatrix();
	const _matrix& GetWorldMatrix();
	const _matrix& GetOrthoMatrix();

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
	ComPtr<ID3D11Texture2D>			m_pTexture_SwapChain = { nullptr };		// 메인 렌더 타겟 텍스처, Deferred의 경우 포지션이 저장됨
	ComPtr<ID3D11RenderTargetView>	m_pRTV_SwapChain = { nullptr };			// 메인 렌더 타겟
	
	// 아래는 디퍼드용 GBuffer 렌더타깃
	ComPtr<ID3D11Texture2D>			m_pTexture_LGC[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };
	ComPtr<ID3D11Texture2D>			m_pTexture_PBR[Cast_EnumDef(ERenderTarget_PBR::Size)] = { nullptr };
	ComPtr<ID3D11Texture2D>			m_pTexture_Common[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };

	ComPtr<ID3D11RenderTargetView>	m_pRTV_LGC[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };		// 레거시 전용 렌더타깃
	ComPtr<ID3D11RenderTargetView>	m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Size)] = { nullptr };		// PBR 전용 렌더타깃
	ComPtr<ID3D11RenderTargetView>	m_pRTV_Common[Cast_EnumDef(ERenderTarget_Common::Size)] = { nullptr };	// 기타 공통 렌더 타깃

	



	using vector_RTV = vector<ComPtr<ID3D11RenderTargetView>>;
	vector_RTV					m_vecRTV;							// 실제 할당되는 렌더타깃의 벡터

	ComPtr<ID3D11Texture2D>			m_pDethStencilBuffer = nullptr;				// CPU용 깊이 버퍼
	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;				// 깊이 스텐실 상태
	ComPtr<ID3D11DepthStencilState>	m_pDepthDisabledStencilState = nullptr;		// Z버퍼 Off한 스텐실 상태
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;				// 깊이 스텐실 뷰

	ComPtr<ID3D11RasterizerState>	m_pRasterState = { nullptr };				// 래스터라이즈 상태 설정
	ComPtr<ID3D11RasterizerState>	m_pRasterCullNoneState = { nullptr };		// 컬을 하지 않는 래스터라이즈

#ifdef _DEBUG
public:
	GETSET_1(ID3D11Debug*, m_pDebug.Get(), Debug, GET__C)

private:
	ComPtr<ID3D11Debug> m_pDebug = { nullptr };
#endif

private:
	XMMATRIX m_matProjection;
	XMMATRIX m_matWorld;
	XMMATRIX m_matOrtho;
};

END