#include "Component/PivotComponent.h"

CPivotComponent::CPivotComponent()
{
    NULL_CHECK(m_pOffsetTransformComp = CTransformComponent::Create());
}

CPivotComponent::CPivotComponent(const CPivotComponent& rhs)
{
    NULL_CHECK(m_pOffsetTransformComp = DynCast<CTransformComponent*>(rhs.m_pOffsetTransformComp->Clone()));
}

HRESULT CPivotComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CPivotComponent::Initialize(void* Arg)
{
	return S_OK;
}

void CPivotComponent::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

void CPivotComponent::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    Transform().Set_Transform(OffsetTransform().Get_TransformMatrix() * Calculate_TransformMatrixFromParentNoSelf());
}

void CPivotComponent::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
}

HRESULT CPivotComponent::Render()
{
	return S_OK;
}

CPivotComponent* CPivotComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("ColliderComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CPivotComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("ColliderComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPivotComponent::Free()
{
    SUPER::Free();

    Safe_Release(m_pOffsetTransformComp);
}
