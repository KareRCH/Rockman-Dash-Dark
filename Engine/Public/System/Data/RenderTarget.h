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
	HRESULT			Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, _float4 vClearColor);

public:
	static CRenderTarget* Create(const DX11DEVICE_T tDevice, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, _float4 vClearColor);

private:
	virtual void	Free();

public:
	HRESULT Bind_ShaderResource(class CEffectComponent* pEffect, const _char* pConstantName);
	HRESULT Clear();

#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_Debug(class CEffectComponent* pEffect, class CVIBufferComp* pVIBuffer);
#endif // _DEBUG

private:
	ComPtr<ID3D11Device> m_pDevice = { nullptr };
	ComPtr<ID3D11DeviceContext> m_pContext = { nullptr };

public:
	ID3D11RenderTargetView* Get_RTV() const { return m_pRTV.Get(); }

private:
	ComPtr<ID3D11Texture2D>	m_pTexture2D = { nullptr };
	ComPtr<ID3D11RenderTargetView> m_pRTV = { nullptr };
	ComPtr<ID3D11ShaderResourceView>	m_pSRV = { nullptr };

	_float4								m_vClearColor = {};

#ifdef _DEBUG
private:
	_float4x4				m_WorldMatrix;

#endif
};

END