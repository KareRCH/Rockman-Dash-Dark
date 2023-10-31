#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphicDev : public CBase
{
	DERIVED_CLASS(CBase, CGraphicDev)
public:
	explicit CGraphicDev();
	virtual ~CGraphicDev() = default;

public:
	HRESULT		Initialize(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
							_float fScreenDepth, _float fScreenNear);
	HRESULT		Clear_BackBuffer_View(_float4 vClearColor);
	HRESULT		Clear_DepthStencil_View();
	HRESULT		Present();

public:
	static CGraphicDev* Create(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
							_float fScreenDepth, _float fScreenNear);

private:
	virtual void Free();

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