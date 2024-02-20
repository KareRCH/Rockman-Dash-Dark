#include "System/TextureMgr.h"

#include "System/Texture.h"

#include "DirectXTex.h"

CTextureMgr::CTextureMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

CTextureMgr* CTextureMgr::Create(const DX11DEVICE_T tDevice, const wstring& strMainPath)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(strMainPath)))
	{
		MSG_BOX("TextureMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CTextureMgr::Free()
{
	for (auto item : m_mapTextures)
	{
		Safe_Release(item.second);
	}
}

HRESULT CTextureMgr::Initialize(const wstring& strMainPath)
{
	m_strMainPath = strMainPath;

	return S_OK;
}

HRESULT CTextureMgr::IsExists_SRV(const wstring& strTextureKey)
{
	auto iter = m_mapTextures.find(strTextureKey);
	if (iter == m_mapTextures.end())
		return E_FAIL;

	if ((*iter).second->Get_TextureCount() == 0)
		return E_NOTIMPL;

	return S_OK;
}

ID3D11ShaderResourceView* CTextureMgr::Find_SRV(const wstring& strTextureKey, const _uint iIndex)
{
	auto iter = m_mapTextures.find(strTextureKey);
	if (iter == m_mapTextures.end())
		return nullptr;

	return (*iter).second->Get_SRV(iIndex);
}

HRESULT CTextureMgr::Reference_SRVs(const wstring& strTextureKey, vector<ComPtr<ID3D11ShaderResourceView>>& RefSRVs)
{
	auto iter = m_mapTextures.find(strTextureKey);
	if (iter == m_mapTextures.end())
		return E_FAIL;

	if (FAILED((*iter).second->Reference_SRVs(RefSRVs)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextureMgr::Load_Texture(const wstring& strFilePath, const _uint iNumTextures, const _bool bPermanent, _bool bUseMainPath)
{
	CTexture* pTexture;
	HRESULT hr = S_OK;

	{
		auto iter = m_mapTextures.find(strFilePath);

		// 텍스처가 없으니 새로만든다.
		if (iter == m_mapTextures.end())
		{
			pTexture = CTexture::Create({ m_pDevice, m_pDeviceContext });
			m_mapTextures.emplace(strFilePath, pTexture);

			if (bUseMainPath)
				hr = pTexture->Insert_Texture(m_strMainPath + strFilePath, iNumTextures, bPermanent);
			else
				hr = pTexture->Insert_Texture(strFilePath, iNumTextures, bPermanent);
		}
		// 텍스처가 있으니 기존에 것에 추가한다.
		else
		{
			pTexture = iter->second;
		}
	}

	return hr;
}
