#include "System/MultiStateTexture.h"

#include <future>
#include <thread>
#include <DirectXTex.h>



CMultiStateTexture::CMultiStateTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	: Base(pDevice, pDeviceContext)
{
}

HRESULT CMultiStateTexture::Initialize()
{
	return S_OK;
}

CMultiStateTexture* CMultiStateTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
	ThisClass* pInstance = new ThisClass(pDevice, pDeviceContext);

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
	for (auto item : m_mapTexture)
	{
		item.second->Free();
	}
}

HRESULT CMultiStateTexture::Reserve(const string& strStateKey, _bool bPermanent)
{
	auto iter = m_mapTexture.find(strStateKey);
	if (iter != m_mapTexture.end())
		return E_FAIL;

	m_mapTexture.emplace(strStateKey, FTextureData::Create(nullptr, nullptr, bPermanent));

	return S_OK;
}

HRESULT CMultiStateTexture::Insert_Texture(const string& strFilePath, const string& strStateKey, const _bool bPermanent)
{
	auto iter = m_mapTexture.find(strFilePath);

	// Ű���� �ִٸ� �ε�� �ؽ�ó���� Ȯ���Ѵ�.
	if (iter != m_mapTexture.end())
	{
		if (!(*iter).second->Is_Loaded())
			return E_FAIL;
	}

	// �ε���� �ʾҴٸ� �ؽ�ó ������ �����Ѵ�.

	// ���� ���翩�� Ȯ��, Ư�� ����� �ش��ϴ� ī��Ʈ ���� �����ϴ��� Ȯ��, �߸��� ��ΰ� �ִٸ� ���� ��ȯ
	_char szFileName[256] = "";
	sprintf(szFileName, strFilePath.c_str());

	ifstream file(szFileName);
	FALSE_CHECK_RETURN(file.good(), E_FAIL);

	_int iWidth = 0;
	_int iHeight = 0;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width = iWidth;
	textureDesc.Height = iHeight;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* pTexture = nullptr;

	FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&textureDesc, NULL, &pTexture), E_FAIL);

	_uint rowPitch = (iWidth * 4) * sizeof(_ubyte);

	//m_pDeviceContext->UpdateSubresource(pTexture, 0, NULL, �ҽ�, rowPitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	ID3D11ShaderResourceView* pTextureView = nullptr;

	FAILED_CHECK_RETURN(m_pDevice->CreateShaderResourceView(pTexture, &srvDesc, &pTextureView), E_FAIL);

	m_pDeviceContext->GenerateMips(pTextureView);

	// �ε��ϰ� ���� �ε��� ������ ������ ��


	// map�� �߰�
	if (iter != m_mapTexture.end())
	{
		// Reserve �� �ؽ�ó�� ���� Permanent�� �������� �ʴ´�.
		(*iter).second->Load(pTexture, pTextureView);
	}
	else
	{
		m_mapTexture.emplace(strStateKey, FTextureData::Create(pTexture, pTextureView, bPermanent));
	}

	return S_OK;
}

void CMultiStateTexture::Transfer_Texture(ID3D11ShaderResourceView* pTexture, const string& strStateKey)
{
	if (m_mapTexture.empty())
		return;

	auto iter = m_mapTexture.find(strStateKey);
	if (iter == m_mapTexture.end())
		return;

	pTexture = (*iter).second->Get_Texture();
}