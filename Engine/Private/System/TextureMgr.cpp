#include "System/TextureMgr.h"

#include "System/MultiStateTexture.h"

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

HRESULT CTextureMgr::Load_Texture(const wstring& strFilePath, const _uint iNumTextures, const _bool bPermanent)
{
	CTexture* pTexture;
	HRESULT hr = S_OK;

	{
		//lock_guard<mutex> lock(m_mapMutex);			// �� �ڵ��ϸ� ��ȣ�Ѵ�.

		size_t iSlash = strFilePath.find_last_of(L"/") + 1;
		wstring strPath = strFilePath.substr(0, iSlash);
		wstring strFileName = strFilePath.substr(iSlash);

		size_t iPeriod = strFileName.find_first_of(L".");
		wstring strName = strFileName.substr(0, iPeriod);

		auto iter = m_mapTextures.find(strFilePath);

		// �ؽ�ó�� ������ ���θ����.
		if (iter == m_mapTextures.end())
		{
			pTexture = CTexture::Create({ m_pDevice, m_pDeviceContext });
			m_mapTextures.emplace(strFilePath, pTexture);
		}
		// �ؽ�ó�� ������ ������ �Ϳ� �߰��Ѵ�.
		else
		{
			pTexture = iter->second;
		}

		hr = pTexture->Insert_Texture(m_strMainPath + strFilePath, iNumTextures, bPermanent);
	}

	return hr;
}

HRESULT CTextureMgr::Load_Texture(const wstring& strFilePath, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent)
{
	//CTexture* pTexture;
	HRESULT hr = S_OK;

	// ���� ���翩�� Ȯ��, �߸��� ��ΰ� �ִٸ� ���� ��ȯ
	//wifstream file(pFilePath);
	//FALSE_CHECK_RETURN(!file.good(), E_FAIL);

	// �ؽ�óŰ�� ���� �ؽ�ó�� �ִ��� ã�ƺ� �� ���ٸ� ���� �����.
	// �ƴϸ� ������ �ؽ�ó ��ü�� ������Ʈ Ű�� �߰��Ѵ�.

	// ���ؽ��� ���� m_mapTexture�� ��ü�� �����ϴ� �Ϳ� ���� ��ȣ�Ѵ�.
	//{
	//	//lock_guard<mutex> lock(m_mapMutex);			// �� �ڵ��ϸ� ��ȣ�Ѵ�.

	//	auto iter = m_mapTextures.find(strGroupKey);

	//	// �ؽ�ó�� ������ ���θ����.
	//	if (iter == m_mapTextures.end())
	//	{
	//		pTexture = CMultiStateTexture::Create({ m_pDevice, m_pDeviceContext });
	//		m_mapTextures.emplace(strGroupKey, pTexture);
	//	}
	//	// �ؽ�ó�� ������ ������ �Ϳ� �߰��Ѵ�.
	//	else
	//	{
	//		pTexture = iter->second;
	//	}
	//}

	//hr = pTexture->Insert_Texture(m_strMainPath + strFilePath, bPermanent);

	return hr;
}
