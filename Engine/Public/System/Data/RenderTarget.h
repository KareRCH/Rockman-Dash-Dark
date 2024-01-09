#pragma once

#include "Base.h"

BEGIN(Engine)

class CRenderTarget : public CBase
{
	DERIVED_CLASS(CBase, CRenderTarget)

private:
	explicit CRenderTarget(const DX11DEVICE_T tDevice);
	explicit CRenderTarget(const CRenderTarget& rhs) = delete;
	virtual ~CRenderTarget() = default;

public:
	HRESULT			Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat);

public:
	static CRenderTarget* Create(const DX11DEVICE_T tDevice, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat);

private:
	virtual void	Free();

private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

private:
	ComPtr<ID3D11Texture2D>	m_pTexture2D = { nullptr };
	ComPtr<ID3D11RenderTargetView> m_pRTV = { nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };
};

END