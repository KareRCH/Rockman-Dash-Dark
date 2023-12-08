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
	HRESULT		Create_RenderTargets();
	// ���� ���ٽ� ����
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
	

private:	// ���� Ÿ��
	ComPtr<ID3D11Texture2D>			m_pTexture[MaxRenderTarget] = { nullptr };
	ComPtr<ID3D11RenderTargetView>	m_pRTV[MaxRenderTarget] = { nullptr };		// 0���� �׻� ����ü�� RTV
	_uint	m_iNumRenderTargets = 0U;

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

private:
	ComPtr<ID3D11Debug> m_pDebug = { nullptr };
#endif

private:
	_uint	m_iScreenWidth = 1280U;
	_uint	m_iScreenHeight = 720U;

};

END