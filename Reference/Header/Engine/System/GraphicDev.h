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
	ComPtr<ID3D11Texture2D>			m_pTexture_SwapChain = { nullptr };		// ���� ���� Ÿ�� �ؽ�ó, Deferred�� ��� �������� �����
	ComPtr<ID3D11RenderTargetView>	m_pRTV_SwapChain = { nullptr };			// ���� ���� Ÿ��
	
	// �Ʒ��� ���۵�� GBuffer ����Ÿ��
	ComPtr<ID3D11Texture2D>			m_pTexture_LGC[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };
	ComPtr<ID3D11Texture2D>			m_pTexture_PBR[Cast_EnumDef(ERenderTarget_PBR::Size)] = { nullptr };
	ComPtr<ID3D11Texture2D>			m_pTexture_Common[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };

	ComPtr<ID3D11RenderTargetView>	m_pRTV_LGC[Cast_EnumDef(ERenderTarget_Legacy::Size)] = { nullptr };		// ���Ž� ���� ����Ÿ��
	ComPtr<ID3D11RenderTargetView>	m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Size)] = { nullptr };		// PBR ���� ����Ÿ��
	ComPtr<ID3D11RenderTargetView>	m_pRTV_Common[Cast_EnumDef(ERenderTarget_Common::Size)] = { nullptr };	// ��Ÿ ���� ���� Ÿ��

	



	using vector_RTV = vector<ComPtr<ID3D11RenderTargetView>>;
	vector_RTV					m_vecRTV;							// ���� �Ҵ�Ǵ� ����Ÿ���� ����

	ComPtr<ID3D11Texture2D>			m_pDethStencilBuffer = nullptr;				// CPU�� ���� ����
	ComPtr<ID3D11DepthStencilState>	m_pDepthStencilState = nullptr;				// ���� ���ٽ� ����
	ComPtr<ID3D11DepthStencilState>	m_pDepthDisabledStencilState = nullptr;		// Z���� Off�� ���ٽ� ����
	ComPtr<ID3D11DepthStencilView>	m_pDepthStencilView = nullptr;				// ���� ���ٽ� ��

	ComPtr<ID3D11RasterizerState>	m_pRasterState = { nullptr };				// �����Ͷ����� ���� ����
	ComPtr<ID3D11RasterizerState>	m_pRasterCullNoneState = { nullptr };		// ���� ���� �ʴ� �����Ͷ�����

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