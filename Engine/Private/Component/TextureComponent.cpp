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
	// �ϴ��� �ؽ�ó �Ŵ����� ���������� �����.
	if (FAILED(Link_TextureManager()))
		return E_FAIL;

	// ���н� �̹� �ִٴ� �Ŵ� ã�´�.
	if (FAILED(m_pTextureMgr->Load_Texture(strFilePath, false)))
	{
		ID3D11ShaderResourceView* pSRV = m_pTextureMgr->Find_SRV(strFilePath);

		// �̰� �ɸ��� ���� ����������
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
	// �̹� �����
	if (nullptr != m_pTextureMgr)
		return S_OK;

	// �ϴ��� �Ŵ����� ���������� ����.
	m_pTextureMgr = GI()->Get_TextureMgr();

	if (nullptr == m_pTextureMgr)
		return E_FAIL;

	Safe_AddRef(m_pTextureMgr);

	return S_OK;
}
