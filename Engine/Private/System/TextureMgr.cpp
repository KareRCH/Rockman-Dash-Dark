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

	// 파일 존재여부 확인, 잘못된 경로가 있다면 오류 반환
	//wifstream file(pFilePath);
	//FALSE_CHECK_RETURN(!file.good(), E_FAIL);

	// 텍스처키로 먼저 텍스처가 있는지 찾아본 후 없다면 새로 만들고.
	// 아니면 기존의 텍스처 객체에 스테이트 키를 추가한다.

	// 뮤텍스를 통해 m_mapTexture에 객체를 제조하는 것에 대해 보호한다.
	{
		//lock_guard<mutex> lock(m_mapMutex);			// 이 코드블록만 보호한다.

		auto iter = m_mapTextureGroup.find(strGroupKey);

		// 텍스처가 없으니 새로만든다.
		if (iter == m_mapTextureGroup.end())
		{
			pTexture = CMultiStateTexture::Create({ m_pDevice, m_pDeviceContext });
			m_mapTextureGroup.emplace(strGroupKey, pTexture);
		}
		// 텍스처가 있으니 기존에 것에 추가한다.
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
	FALSE_CHECK_RETURN(iter != m_mapTextureGroup.end(), E_FAIL);		// 키 없으면 실패

	CTexture* pTexture = iter->second;

	CMultiStateTexture* pMultiTex = DynCast<CMultiStateTexture*>(pTexture);
	//if (pMultiTex)
		//pMultiTex->Transfer_Texture(pVecTexture, pStateKey);

	

	return S_OK;
}
