#include "Component/TextureComponent.h"

#include "System/GameInstance.h"

CTextureComponent::CTextureComponent(const CTextureComponent& rhs)
    : Base(rhs)
	, m_pTexture(rhs.m_pTexture), m_pSRV(rhs.m_pSRV)
{
}

HRESULT CTextureComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CTextureComponent::Initialize(void* Arg)
{
    return S_OK;
}

CTextureComponent* CTextureComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
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

	if (FAILED(pInstance->Initialize()))
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

HRESULT CTextureComponent::Bind_TextureFromManager(const wstring& strFilePath)
{
	ID3D11ShaderResourceView* pSRV = GI()->Find_SRV(strFilePath);
	if (!pSRV)
		return E_FAIL;

	m_pSRV = pSRV;

	return S_OK;
}
