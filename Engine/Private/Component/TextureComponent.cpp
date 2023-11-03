#include "Component/TextureComponent.h"

CTextureComponent::CTextureComponent(ID3D11Device* pDevice)
    : Base(pDevice)
{
}

CTextureComponent::CTextureComponent(const CTextureComponent& rhs)
    : Base(rhs)
{
}

HRESULT CTextureComponent::Initialize()
{
    return S_OK;
}

_int CTextureComponent::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTextureComponent::LateTick()
{
}

void CTextureComponent::Render(ID3D11DeviceContext* pDeviceContext)
{
}

CTextureComponent* CTextureComponent::Create(ID3D11Device* pDevice)
{
	ThisClass* pInstance = new ThisClass(pDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TextureComponent Create Failed");

		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CTextureComponent::Clone()
{
    return new ThisClass(*this);
}

void CTextureComponent::Free()
{
    SUPER::Free();
}
