#include "RenderTarget.h"

CRenderTarget::CRenderTarget(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pContext(tDevice.pDeviceContext)
{
}

HRESULT CRenderTarget::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat)
{
	ComPtr<ID3D11Texture2D> pTexture2D = { nullptr };

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

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, pTexture2D.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pTexture2D.Get(), nullptr, m_pRTV.GetAddressOf())))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(pTexture2D.Get(), nullptr, m_pSRV.GetAddressOf())))
		return E_FAIL;

	m_pTexture2D = pTexture2D;

	return S_OK;
}

CRenderTarget* CRenderTarget::Create(const DX11DEVICE_T tDevice, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(iSizeX, iSizeY, ePixelFormat)))
	{
		MSG_BOX("RenderMgr Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	m_pTexture2D.Reset();
	m_pRTV.Reset();
	m_pSRV.Reset();
	m_pDevice.Reset();
	m_pContext.Reset();
}
