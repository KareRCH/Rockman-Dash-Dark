#include "Component/EffectComponent.h"

#include "System/GameInstance.h"

CEffectComponent::CEffectComponent(const CEffectComponent& rhs)
    : Base(rhs)
    , m_pDeviceComp(rhs.m_pDeviceComp)
    , m_pEffect(rhs.m_pEffect)
    , m_InputLayouts(rhs.m_InputLayouts)
    , m_TechniqueDesc(rhs.m_TechniqueDesc)
{
}

HRESULT CEffectComponent::Initialize_Prototype(void* Arg)
{
    FEffectCompDesc tDesc;

    if (Arg)
        tDesc = (*ReCast<FEffectCompDesc*>(Arg));

    //m_pEffect = 

    m_pDeviceComp = CD3D11DeviceComp::Create();

	return S_OK;
}

HRESULT CEffectComponent::Initialize(void* Arg)
{
	return S_OK;
}

CEffectComponent* CEffectComponent::Create(FEffectCompDesc tDesc)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(VPCast(&tDesc))))
    {
        MSG_BOX("EffectComponent Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CEffectComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("EffectComponent Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CEffectComponent::Free()
{

}
