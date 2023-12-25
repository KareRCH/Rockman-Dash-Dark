#include "Component/MaterialComponent.h"

#include "Component/TextureComponent.h"
#include "System/Data/MaterialData.h"
#include "Component/EffectComponent.h"

CMaterialComponent::CMaterialComponent()
{
}

CMaterialComponent::CMaterialComponent(const CMaterialComponent& rhs)
    : Base(rhs)
	, m_arrTextureComps(rhs.m_arrTextureComps)
{
	for (_uint i = 1; i < g_iNumTextures; i++)
	{
		Safe_AddRef(m_arrTextureComps[i]);
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
	SUPER::Free();

	for (auto& pTexture : m_arrTextureComps)
		Safe_Release(pTexture);
	
}

HRESULT CMaterialComponent::Load_Materials(EModelGroupIndex eGroupIndex, const wstring& strModelKey, _uint iIndex)
{
	if (nullptr == m_pGI)
		return E_FAIL;

	auto pMaterialData = m_pGI->Find_MaterialData(eGroupIndex, strModelKey, iIndex);
	if (nullptr == pMaterialData)
		return E_FAIL;

	for (_uint i = 1; i < g_iNumTextures; i++)
	{
		// 머터리얼의 존재여부가 있을 때 텍스처를 생성한다.
		if (!pMaterialData->strTexture[i].empty())
		{
			m_arrTextureComps[i] = CTextureComponent::Create();
			if (FAILED(m_arrTextureComps[i]->Bind_Texture(pMaterialData->strTexture[i], iIndex)))
			{
				// 로드 실패하면 죽이기
				Safe_Release(m_arrTextureComps[i]);
			}
		}
	}

	return S_OK;
}

ID3D11ShaderResourceView* CMaterialComponent::Get_SRV(_uint iIndex)
{
	if (iIndex < 1 || iIndex >= g_iNumTextures)
		return nullptr;

	if (nullptr == m_arrTextureComps[iIndex])
		return nullptr;

	return m_arrTextureComps[iIndex]->Get_ShaderResourseView(iIndex);
}

HRESULT CMaterialComponent::Bind_TextureToEffect(CEffectComponent* pEffectComp, const _char* pConstantName, _uint iTextureIndex)
{
	if (nullptr == pEffectComp)
		return E_FAIL;

	if (nullptr == m_arrTextureComps[iTextureIndex])
		return E_FAIL;

	return pEffectComp->Bind_SRV(pConstantName, m_arrTextureComps[iTextureIndex]->Get_ShaderResourseView(0));
}
