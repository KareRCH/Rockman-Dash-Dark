#pragma once

#include "Base.h"

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
	HRESULT		Ready_Depth(const FDEVICE_INIT& tInit);
	HRESULT		Ready_Stencil(const FDEVICE_INIT& tInit);
	HRESULT		Ready_Viewport(const FDEVICE_INIT& tInit);

public:
	GETSET_1(ID3D11Device*, m_pDevice, Device, GET_REF)
	GETSET_1(ID3D11DeviceContext*, m_pDeviceContext, DeviceContext, GET_REF)

private:
	_bool	m_bVsync_Enabled = false;
	_int	m_iVideoCardMemory = 0;
	_char	m_cVideoCardDescription[128] = {0};

	IDXGISwapChain*				m_pSwapChain = nullptr;
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;
	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11Texture2D*			m_pDethStencilBuffer = nullptr;
	ID3D11DepthStencilState*	m_pDepthStencilState = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;
	ID3D11RasterizerState*		m_pRasterState = nullptr;

private:
	XMMATRIX m_matProjection;
	XMMATRIX m_matWorld;
	XMMATRIX m_matOrtho;
};

END