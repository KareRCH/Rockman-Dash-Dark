#include "System/MultiStateTexture.h"

#include <future>
#include <thread>

#include <DirectXTex.h>

CMultiStateTexture::CMultiStateTexture(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

HRESULT CMultiStateTexture::Initialize()
{
	return S_OK;
}

CMultiStateTexture* CMultiStateTexture::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);

		MSG_BOX("Texture Create Failed");
		return nullptr;
	}

	return pInstance;
}

void CMultiStateTexture::Free()
{
	SUPER::Free();

	for (auto item : m_mapTexture)
	{
		item.second->Free();
	}
}

HRESULT CMultiStateTexture::Reserve(const wstring& strTextureKey, _bool bPermanent)
{
	auto iter = m_mapTexture.find(strTextureKey);
	if (iter != m_mapTexture.end())
		return E_FAIL;

	m_mapTexture.emplace(strTextureKey, FTextureData::Create(nullptr, nullptr, bPermanent));

	return S_OK;
}

ID3D11ShaderResourceView* CMultiStateTexture::Find_SRV(const wstring& strTextureKey)
{
	auto iter = m_mapTexture.find(strTextureKey);
	if (iter == m_mapTexture.end())
		return nullptr;

	return (*iter).second->Get_SRV();
}

HRESULT CMultiStateTexture::Insert_Texture(const wstring& strFilePath, const wstring& strTextureKey, const _bool bPermanent)
{
	auto iter = m_mapTexture.find(strFilePath);

	// 키값이 있다면 로드된 텍스처인지 확인한다.
	if (iter != m_mapTexture.end())
	{
		if (!(*iter).second->Is_Loaded())
			return E_FAIL;
	}

	// 파일 존재여부 확인, 특히 사이즈에 해당하는 카운트 값이 존재하는지 확인, 잘못된 경로가 있다면 오류 반환
	_tchar szFileName[256] = L"";
	wsprintf(szFileName, strFilePath.c_str());

	ifstream file(szFileName);
	if (file.fail())
		return E_FAIL;

	ScratchImage image;
	HRESULT hr = S_OK;
	hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	if (FAILED(hr))
		return E_FAIL;

	hr = LoadFromWICFile((strFilePath).c_str(), WIC_FLAGS_NONE, nullptr, image);
	if (FAILED(hr))
		return E_FAIL;

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

	ID3D11ShaderResourceView* pTextureView = { nullptr };
	hr = CreateShaderResourceView(m_pDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &pTextureView);
	if (FAILED(hr))
	{
		Safe_Release(pTextureView);
		return E_FAIL;
	}

	// map에 추가
	if (iter != m_mapTexture.end())
	{
		// Reserve 된 텍스처는 따로 Permanent를 설정하지 않는다.
		(*iter).second->Load(pTexture, pTextureView);
	}
	// 로드되지 않았다면 텍스처 삽입을 진행한다.
	else
	{
		m_mapTexture.emplace(strTextureKey, FTextureData::Create(pTexture, pTextureView, bPermanent));
	}

	return S_OK;
}

void CMultiStateTexture::Transfer_SRV(ID3D11ShaderResourceView* pTexture, const wstring& strTextureKey)
{
	if (m_mapTexture.empty())
		return;

	auto iter = m_mapTexture.find(strTextureKey);
	if (iter == m_mapTexture.end())
		return;

	pTexture = (*iter).second->Get_SRV();
}