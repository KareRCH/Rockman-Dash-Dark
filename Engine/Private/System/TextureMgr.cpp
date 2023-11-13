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
	for (auto item : m_mapTextureGroup)
	{
		Safe_Release(item.second);
	}
}

HRESULT CTextureMgr::Initialize(const wstring& strMainPath)
{
	m_strMainPath = strMainPath;

	return S_OK;
}

ID3D11ShaderResourceView* CTextureMgr::Get_Texture(const wstring& strGroupKey, const wstring& strTextureKey)
{
	auto iter = m_mapTextureGroup.find(strGroupKey);
	if (iter == m_mapTextureGroup.end())
		return nullptr;

	return (*iter).second->Get_Texture(strTextureKey);
}

HRESULT CTextureMgr::Load_Texture(const wstring& strFilePath, const wstring& strGroupKey, const wstring& strTextureKey, const _bool bPermanent)
{
	CTexture* pTexture;
	HRESULT hr = S_OK;

	// ���� ���翩�� Ȯ��, �߸��� ��ΰ� �ִٸ� ���� ��ȯ
	//wifstream file(pFilePath);
	//FALSE_CHECK_RETURN(!file.good(), E_FAIL);

	// �ؽ�óŰ�� ���� �ؽ�ó�� �ִ��� ã�ƺ� �� ���ٸ� ���� �����.
	// �ƴϸ� ������ �ؽ�ó ��ü�� ������Ʈ Ű�� �߰��Ѵ�.

	// ���ؽ��� ���� m_mapTexture�� ��ü�� �����ϴ� �Ϳ� ���� ��ȣ�Ѵ�.
	{
		//lock_guard<mutex> lock(m_mapMutex);			// �� �ڵ��ϸ� ��ȣ�Ѵ�.

		auto iter = m_mapTextureGroup.find(strGroupKey);

		// �ؽ�ó�� ������ ���θ����.
		if (iter == m_mapTextureGroup.end())
		{
			pTexture = CMultiStateTexture::Create({ m_pDevice, m_pDeviceContext });
			m_mapTextureGroup.emplace(strGroupKey, pTexture);
		}
		// �ؽ�ó�� ������ ������ �Ϳ� �߰��Ѵ�.
		else
		{
			pTexture = iter->second;
		}
	}

	hr = pTexture->Insert_Texture(m_strMainPath + strFilePath, strTextureKey, bPermanent);
	if (FAILED(hr))
		return E_FAIL;

	return hr;
}

HRESULT CTextureMgr::Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, const wstring& strGroupKey, const wstring& strTextureKey)
{
	auto iter = m_mapTextureGroup.find(strGroupKey);
	FALSE_CHECK_RETURN(iter != m_mapTextureGroup.end(), E_FAIL);		// Ű ������ ����

	CTexture* pTexture = iter->second;

	CMultiStateTexture* pMultiTex = DynCast<CMultiStateTexture*>(pTexture);
	//if (pMultiTex)
		//pMultiTex->Transfer_Texture(pVecTexture, pStateKey);

	

	return S_OK;
}
