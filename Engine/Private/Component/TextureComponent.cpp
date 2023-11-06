#include "Component/TextureComponent.h"

CTextureComponent::CTextureComponent(const DX11DEVICE_T tDevice)
    : Base(tDevice)
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

void CTextureComponent::PriorityTick()
{
}

_int CTextureComponent::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTextureComponent::LateTick()
{
}

void CTextureComponent::Render()
{
}

CTextureComponent* CTextureComponent::Create(const DX11DEVICE_T tDevice)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("TextureComponent Create Failed");

		return nullptr;
	}

	return pInstance;
}

CPrimitiveComponent* CTextureComponent::Clone(void* Arg)
{
    return new ThisClass(*this);
}

void CTextureComponent::Free()
{
    SUPER::Free();
}
