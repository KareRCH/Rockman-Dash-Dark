#include "Component/TextureComponent.h"

#include "System/GameInstance.h"
#include "System/TextureMgr.h"
#include "Component/EffectComponent.h"

CTextureComponent::CTextureComponent(const CTextureComponent& rhs)
    : Base(rhs)
	, m_pTextureMgr(rhs.m_pTextureMgr), m_vecSRVs(rhs.m_vecSRVs)
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
	}

	return Cast<CComponent*>(pInstance);
}

void CTextureComponent::Free()
{
	SUPER::Free();

	Safe_Release(m_pTextureMgr);
}

ID3D11ShaderResourceView* CTextureComponent::Get_ShaderResourseView(const _uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumTextures)
		return nullptr;

	return m_vecSRVs[iIndex].Get();
}

HRESULT CTextureComponent::Get_ShaderResourceViews(ID3D11ShaderResourceView** Arr, const _uint iNumTextures)
{
	// �ؽ�ó�� ���ų�, ����� ���� ������ ����Ѵ�.
	if (m_iNumTextures == 0 || m_iNumTextures != iNumTextures)
		return E_FAIL;

	for (_uint i = 0; i < iNumTextures; i++)
		Arr[i] = m_vecSRVs[i].Get();

	return S_OK;
}

HRESULT CTextureComponent::Bind_Texture(const wstring& strFilePath, const _uint iNumTextures)
{
	if (iNumTextures == 0)
	{
		MSG_BOX("�̻��� �� ���� ���ÿ�~");
		return E_FAIL;
	}

	// �ϴ��� �ؽ�ó �Ŵ����� ���������� �����.
	if (FAILED(Link_TextureManager()))
		return E_FAIL;

	// ���н� �̹� �ִٴ� �Ŵ� ã�´�.
	if (FAILED(m_pTextureMgr->Load_Texture(strFilePath, iNumTextures, false)))
	{
		// �ε� �����ϸ� �׳� ������ �������� ã�°Ŵ�.
	}

	// ã�Ҵµ� ������ ġ������ ����
	if (FAILED(m_pTextureMgr->Reference_SRVs(strFilePath, m_vecSRVs)))
		return E_FAIL;

	// ������ �ؽ�ó ������ ������Ʈ
	m_iNumTextures = Cast<_uint>(m_vecSRVs.size());

	return S_OK;
}

HRESULT CTextureComponent::Unbind_Texture()
{
	for (auto& SRV : m_vecSRVs)
		SRV.Reset();
	m_vecSRVs.clear();
	m_iNumTextures = 0;

	return S_OK;
}

HRESULT CTextureComponent::Bind_SRVToEffect(CEffectComponent* pEffect, const _char* pTextureSymbolName, const _uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumTextures)
		return E_FAIL;

	return pEffect->Bind_SRV(pTextureSymbolName, m_vecSRVs[iIndex].Get());
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
