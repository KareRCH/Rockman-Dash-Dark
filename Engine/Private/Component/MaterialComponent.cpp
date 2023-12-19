#include "Component/MaterialComponent.h"

#include "Component/TextureComponent.h"

CMaterialComponent::CMaterialComponent()
{
}

CMaterialComponent::CMaterialComponent(const CMaterialComponent& rhs)
    : Base(rhs)
	, m_vecTextureComp(rhs.m_vecTextureComp)
{
	for (_uint i = 1; i < g_iNumTextures; i++)
	{
		Safe_AddRef(m_vecTextureComp[i]);
	}
}

HRESULT CMaterialComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CMaterialComponent::Initialize(void* Arg)
{
    return S_OK;
}

CMaterialComponent* CMaterialComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CMaterialComponent Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMaterialComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("CMaterialComponent Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CMaterialComponent::Free()
{
	for (auto& pTexture : m_vecTextureComp)
	{
		Safe_Release(pTexture);
	}
}

ID3D11ShaderResourceView* CMaterialComponent::Get_Texture(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= g_iNumTextures)
		return nullptr;

	if (nullptr == m_vecTextureComp[iIndex])
		return nullptr;

	return m_vecTextureComp[iIndex]->Get_ShaderResourseView();
}
