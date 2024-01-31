#include "RenderTarget.h"

#include "Component/EffectComponent.h"
#include "Component/VIBufferComp.h"

CRenderTarget::CRenderTarget(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pContext(tDevice.pDeviceContext)
{
}

HRESULT CRenderTarget::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, _float4 vClearColor)
{
	m_vClearColor = vClearColor;

	D3D11_TEXTURE2D_DESC	TextureDesc = {};

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = ePixelFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, m_pTexture2D.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D.Get(), nullptr, m_pRTV.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D.Get(), nullptr, m_pSRV.GetAddressOf())))
		return E_FAIL;

	return S_OK;
}

CRenderTarget* CRenderTarget::Create(const DX11DEVICE_T tDevice, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, _float4 vClearColor)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(iSizeX, iSizeY, ePixelFormat, vClearColor)))
	{
		MSG_BOX("RenderMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
}

HRESULT CRenderTarget::Bind_ShaderResource(CEffectComponent* pEffect, const _char* pConstantName)
{
	return pEffect->Bind_SRV(pConstantName, m_pSRV.Get());
}

HRESULT CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV.Get(), (_float*)&m_vClearColor);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;

	D3D11_VIEWPORT		Viewport;
	_uint				iNumViewport = { 1 };

	m_pContext->RSGetViewports(&iNumViewport, &Viewport);

	m_WorldMatrix._41 = fX - Viewport.Width * 0.5f;
	m_WorldMatrix._42 = -fY + Viewport.Height * 0.5f;

	return S_OK;
}

HRESULT CRenderTarget::Render_Debug(CEffectComponent* pEffect, CVIBufferComp* pVIBuffer)
{
	if (FAILED(pEffect->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(pEffect->Bind_SRV("g_DiffuseTexture", m_pSRV.Get())))
		return E_FAIL;

	pEffect->Begin(0);

	pVIBuffer->Bind_Buffer();

	return pVIBuffer->Render_Buffer();
}
#endif // _DEBUG

HRESULT CRenderTarget::Resize_RenderTarget(_uint iResizeWidth, _uint iResizeHeight)
{
	D3D11_TEXTURE2D_DESC Desc = {};
	m_pTexture2D->GetDesc(&Desc);
	Desc.Width = iResizeWidth;
	Desc.Height = iResizeHeight;

	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
	m_pRTV->GetDesc(&RTVDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	m_pSRV->GetDesc(&SRVDesc);

	ComPtr<ID3D11Texture2D> pTexture = { nullptr };
	if (FAILED(m_pDevice->CreateTexture2D(&Desc, nullptr, pTexture.GetAddressOf())))
	{
		return E_FAIL;
	}

	ComPtr<ID3D11ShaderResourceView> pSRV = { nullptr };
	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture.Get(), &SRVDesc, pSRV.GetAddressOf())))
	{
		return E_FAIL;
	}

	ComPtr<ID3D11RenderTargetView> pRTV = { nullptr };
	if (FAILED(m_pDevice->CreateRenderTargetView(pTexture.Get(), &RTVDesc, pRTV.GetAddressOf())))
	{
		return E_FAIL;
	}

	m_pTexture2D = pTexture;
	m_pRTV = pRTV;
	m_pSRV = pSRV;

	return S_OK;
}


