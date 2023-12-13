#include "Component/TextureComponent.h"

#include "System/GameInstance.h"
#include "System/TextureMgr.h"
#include "Component/EffectComponent.h"

CTextureComponent::CTextureComponent(const CTextureComponent& rhs)
    : Base(rhs)
	, m_pTextureMgr(rhs.m_pTextureMgr), m_pSRV(rhs.m_pSRV)
{
	Safe_AddRef(m_pTextureMgr);
}

HRESULT CTextureComponent::Initialize_Prototype(void* Arg)
{
	if (FAILED(Link_TextureManager()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTextureComponent::Initialize(void* Arg)
{
    return S_OK;
}

CTextureComponent* CTextureComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("TextureComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CTextureComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("TextureComponent Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTextureComponent::Free()
{

}

HRESULT CTextureComponent::Bind_Texture(const wstring& strFilePath)
{
	// 일단은 텍스처 매니저에 종속적으로 만든다.
	if (FAILED(Link_TextureManager()))
		return E_FAIL;

	// 실패시 이미 있다는 거니 찾는다.
	if (FAILED(m_pTextureMgr->Load_Texture(strFilePath, false)))
	{
		ID3D11ShaderResourceView* pSRV = m_pTextureMgr->Find_SRV(strFilePath);

		// 이거 걸리면 문제 개빡센거임
		if (nullptr == pSRV)
			return E_FAIL;

		m_pSRV = pSRV;
	}

	return S_OK;
}

HRESULT CTextureComponent::Bind_TextureFromManager(const wstring& strFilePath)
{
	if (FAILED(Link_TextureManager()))
		return E_FAIL;

	ID3D11ShaderResourceView* pSRV = m_pTextureMgr->Find_SRV(strFilePath);
	if (!pSRV)
		return E_FAIL;

	m_pSRV = pSRV;

	return S_OK;
}

HRESULT CTextureComponent::Unbind_Texture()
{
	m_pSRV.Reset();

	return S_OK;
}

HRESULT CTextureComponent::Bind_SRV(CEffectComponent* pEffect, const _char* pTextureName)
{
	return pEffect->Bind_SRV(pTextureName, m_pSRV.Get());
}

HRESULT CTextureComponent::Link_TextureManager()
{
	// 이미 연결됨
	if (nullptr != m_pTextureMgr)
		return S_OK;

	// 일단은 매니저에 종속적으로 만듬.
	m_pTextureMgr = GI()->Get_TextureMgr();

	if (nullptr == m_pTextureMgr)
		return E_FAIL;

	Safe_AddRef(m_pTextureMgr);

	return S_OK;
}
