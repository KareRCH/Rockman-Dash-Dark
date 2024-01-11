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
/// �׷�����ġ�� �ʱ�ȭ�ϴ� Ŭ����
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
	// ���� Ÿ�� ���տ� �߰��ϱ�
	HRESULT		Regist_RenderTarget(_uint iRenderTargetIndex);
	// ��ġ�� ����Ÿ�� ���ε�
	void		Bind_RenderTargetsOnDevice();

private:
	// ���� Ÿ�� �ı�
	void		CleanUp_RenderTargets();
	// ���� Ÿ�� ����, �������
	HRESULT		Create_RenderTargets(_bool bIsInit);
	// ���� ���ٽ� ����
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
	// ����Ʈ ������� ����Ÿ�ٿ� ��ϵ� �������� �ؽ�ó ���ҽ��� �����ؿ��� �Լ�. ������ ����.
	HRESULT		Copy_RenderTargetViewToTexture_ByViewport(ComPtr<ID3D11Texture2D>& pTexture, _uint iRenderTargetIndex, _uint iViewportIndex);
	// ����Ʈ ������� ����ۿ� ��ϵ� �ȼ����� ���̴� ���ҽ��� �����ؿ��� �Լ�
	HRESULT		Copy_BackBufferToSRV_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, _uint iViewportIndex);
	HRESULT		Copy_BackBufferToSRV_ByViewport(ComPtr<ID3D11ShaderResourceView>& pSRV, const D3D11_VIEWPORT Viewport);
	// ����Ÿ�� ������ �ҷ��� ������ �����ϴµ� �� �� �ִ�.
	const D3D11_TEXTURE2D_DESC* Get_RTV_Desc(_uint iIndex) const;
	// ����Ÿ�� ��������, ����Ÿ�ٵ� ���� �ٲ��.
	void						Set_RTV_Desc(_uint iIndex, D3D11_TEXTURE2D_DESC& Desc);
	ID3D11RenderTargetView*		Get_BackBufferRTV() const { return m_pRTV[0].Get(); }
	ID3D11DepthStencilView*		Get_DSV() const { return m_pDepthStencilView.Get(); }
	

private:	// ���� Ÿ��
	D3D11_RENDER_TARGET_VIEW_DESC	m_RTV_Descs[MaxRenderTarget] = {};			// ���� Ÿ�� �������� �����Ѵ�.
	D3D11_TEXTURE2D_DESC			m_RTVTexture_Descs[MaxRenderTarget] = {};	// ���� Ÿ���� �ؽ�ó ������ ������ �� �ִ�.
	ComPtr<ID3D11RenderTargetView>	m_pRTV[MaxRenderTarget] = { nullptr };		// 0���� �׻� ����ü�� RTV
	_uint							m_iNumRenderTargets = 0U;					// ����Ÿ�� ��

	using vector_RTV = vector<ID3D11RenderTargetView*>;
	vector_RTV						m_vecRTV;									// ���� �Ҵ�Ǵ� ����Ÿ���� ����

private:	// ���ٽ�
	ComPtr<ID3D11Texture2D>			m_pDepthStencilBuffer = nullptr;			// ���ٽ� �� ������ ����
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;				// ���� ���ٽ� ��

	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;				// �Ϲ� ���ٽ� ����
	ComPtr<ID3D11DepthStencilState>	m_pDepthDisabledStencilState = nullptr;		// Z���� Off�� ���ٽ� ����

	ComPtr<ID3D11RasterizerState>	m_pRasterState = { nullptr };				// �Ϲ� �����Ͷ����� ����
	ComPtr<ID3D11RasterizerState>	m_pRasterCullNoneState = { nullptr };		// ���� ���� �ʴ� �����Ͷ�����


public:		// �ý��� ������ ���� ���̴� ����Ʈ �����̴�.
	void Add_Viewport(D3D11_VIEWPORT Viewport);
	void Set_Viewport(_uint iIndex, D3D11_VIEWPORT Viewport);
	D3D11_VIEWPORT* Get_ViewportPtr(_uint iIndex);
	// ����Ʈ�� �����Ѵ�.
	HRESULT Bind_Viewport();
	HRESULT Bind_Viewport(_uint iIndex);

private:	// �ý��� �������� ����Ʈ
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

private:	// â ũ��, �ػ󵵿� ������ ���� �ʽ��ϴ�.
	_uint			m_iScreenWidth = g_iWindowSizeX;
	_uint			m_iScreenHeight = g_iWindowSizeY;
	_float			m_iScreenRatio = 1.f;

public:		// �ػ� ���� ����
	void			Set_Resolution(_uint iX, _uint iY);
	void			Set_ResolutionX_MaintainRatio(_uint iX);
	void			Set_ResolutionY_MaintainRatio(_uint iY);
	const _uint&	Get_ResolutionX() const { return m_viResolutionX; }
	const _uint&	Get_ResolutionY() const { return m_viResolutionY; }
	const _float&	Get_ResolutionRatio() const { return m_fResolutionRatio; }

private:	// �ػ󵵴� ���� ������ ǥ�õǴ� ���� ������ �ݴϴ�. ������� ũ�⺸�� �۰ų� Ŭ �� �ֽ��ϴ�.
	_uint			m_viResolutionX = g_iWindowSizeX;
	_uint			m_viResolutionY = g_iWindowSizeY;
	_float			m_fResolutionRatio = 1.f;

public:
	const _float&	Get_ResolutionByScreen_RatioX() { return m_vResolutionByScreen_Ratio.x; }
	const _float&	Get_ResolutionByScreen_RatioY() { return m_vResolutionByScreen_Ratio.y; }

private:			// �ػ� : ȭ���, ���� ǥ�õǴ� ������ �����ϱ� ���� ���˴ϴ�.
	void			Apply_ResolutionByScreen_Ratio();

private:
	_float2			m_vResolutionByScreen_Ratio = {};

};

END