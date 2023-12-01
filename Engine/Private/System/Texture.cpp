#include "System/Texture.h"

#include <future>
#include <thread>

#include <DirectXTex.h>

CTexture::CTexture(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CTexture::Initialize()
{
	return S_OK;
}

CTexture* CTexture::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Texture Create Failed");
		Safe_Release(pInstance);
		
		return nullptr;
	}

	return pInstance;
}

void CTexture::Free()
{
}

void CTexture::UnLoad()
{
	m_pTexture.Reset();
	m_pSRV.Reset();
	m_bLoaded = false;
}

HRESULT CTexture::Load(ID3D11Texture2D* _pTexture, ID3D11ShaderResourceView* _pSRV)
{
	if (m_bLoaded)
	{
		// ���� ������ �ڵ����� �����Ѵ�.
		Safe_Release(_pTexture);
		Safe_Release(_pSRV);

		return E_FAIL;
	}

	m_pTexture = _pTexture;
	m_pSRV = _pSRV;

	// ���� �ϳ��� nullptr�̸� �ε���� �ʾ���.
	if (m_pTexture.Get() && m_pSRV.Get())
		m_bLoaded = true;

	return S_OK;
}

HRESULT CTexture::Insert_Texture(const wstring& strFilePath, const wstring& strName, const _bool bPermanent)
{
	// ���� ���翩�� Ȯ��, Ư�� ����� �ش��ϴ� ī��Ʈ ���� �����ϴ��� Ȯ��, �߸��� ��ΰ� �ִٸ� ���� ��ȯ
	_tchar szFileName[256] = L"";
	wsprintf(szFileName, strFilePath.c_str());

	ifstream file(szFileName);
	if (file.fail())
		return E_FAIL;

	// �ؽ�ó �ε�
	ScratchImage image;
	HRESULT hr = S_OK;
	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return E_FAIL;

	hr = LoadFromWICFile((strFilePath).c_str(), WIC_FLAGS_NONE, nullptr, image);
	if (FAILED(hr))
		return E_FAIL;

	// �ؽ�ó, SRV ����
	const Image* texData = image.GetImage(0, 0, 0);
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = Cast<_uint>(texData->width);
	textureDesc.Height = Cast<_uint>(texData->height);
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = texData->format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	ID3D11Texture2D* pTexture = nullptr;
	hr = m_pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);
	if (FAILED(hr))
	{
		Safe_Release(pTexture);
		return E_FAIL;
	}

	m_pDeviceContext->UpdateSubresource(pTexture, 0, nullptr, texData->pixels, Cast<_uint>(texData->rowPitch), 0);

	ID3D11ShaderResourceView* pSRV = { nullptr };
	hr = CreateShaderResourceView(m_pDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pSRV);
	if (FAILED(hr))
	{
		Safe_Release(pSRV);
		return E_FAIL;
	}

	// �ε�� ���ҽ� �Ҵ�
	Load(pTexture, pSRV);
	Set_Permanent(bPermanent);
	m_strName = strName;

	return S_OK;
}
