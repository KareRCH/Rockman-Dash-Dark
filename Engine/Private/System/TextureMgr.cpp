#include "System/TextureMgr.h"

#include "System/MultiStateTexture.h"

CTextureMgr::CTextureMgr()
{
}

CTextureMgr* CTextureMgr::Create(ID3D11Device* pGraphicDev)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(pGraphicDev)))
	{
		MSG_BOX("TextureMgr Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CTextureMgr::Free()
{
	for (auto item : m_mapTexture)
	{
		Safe_Release(item.second);
	}
}

HRESULT CTextureMgr::Initialize(ID3D11Device* pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;

	return S_OK;
}

HRESULT CTextureMgr::Load_Texture(TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey, const _range<_uint>& iCntRange)
{
	

	return S_OK;
}

HRESULT CTextureMgr::Insert_Texture(const _tchar* pFilePath, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey, const _range<_uint>& iCntRange)
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
		lock_guard<mutex> lock(m_mapMutex);			// �� �ڵ��ϸ� ��ȣ�Ѵ�.

		auto iter = m_mapTexture.find(pTextureKey);

		// �ؽ�ó�� ������ ���θ����.
		if (iter == m_mapTexture.end())
		{
			pTexture = CMultiStateTexture::Create(m_pGraphicDev);
			m_mapTexture.emplace(pTextureKey, pTexture);
		}
		// �ؽ�ó�� ������ ������ �Ϳ� �߰��Ѵ�.
		else
		{
			pTexture = iter->second;
		}
	}

	//FAILED_CHECK_RETURN(hr = pTexture->Insert_Texture(pFilePath, eType, pStateKey, iCntRange), E_FAIL);

	return hr;
}

HRESULT CTextureMgr::Transfer_Texture(vector<ID3D11Texture2D>* pVecTexture, TEXTUREID eType, const _tchar* pTextureKey, const _tchar* pStateKey)
{
	auto iter = m_mapTexture.find(pTextureKey);
	FALSE_CHECK_RETURN(iter != m_mapTexture.end(), E_FAIL);		// Ű ������ ����

	CTexture* pTexture = iter->second;

	CMultiStateTexture* pMultiTex = dynamic_cast<CMultiStateTexture*>(pTexture);
	//if (pMultiTex)
		//pMultiTex->Transfer_Texture(pVecTexture, pStateKey);

	return S_OK;
}
