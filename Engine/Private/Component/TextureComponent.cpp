#include "Component/TextureComponent.h"

CTextureComponent::CTextureComponent(const CTextureComponent& rhs)
    : Base(rhs)
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

void CTextureComponent::Priority_Tick(const _float& fTimeDelta)
{
}

_int CTextureComponent::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTextureComponent::Late_Tick(const _float& fTimeDelta)
{
}

void CTextureComponent::Render()
{
}

CTextureComponent* CTextureComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("TextureComponent Create Failed");

		return nullptr;
	}

	return pInstance;
}

CComponent* CTextureComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);
		MSG_BOX("TextureComponent Copy Failed");

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTextureComponent::Free()
{
    SUPER::Free();
}
