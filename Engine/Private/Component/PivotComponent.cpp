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

HRESULT CPivotComponent::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    m_pOffsetTransformComp->Set_Transform(Transform().Get_TransformMatrix());

    return S_OK;
}

HRESULT CPivotComponent::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CPivotComponent::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

    m_pOffsetTransformComp->Set_Transform(Transform().Get_TransformMatrix());

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

FSerialData CPivotComponent::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

FSerialData CPivotComponent::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

CPivotComponent* CPivotComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("PivotComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CPivotComponent* CPivotComponent::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("PivotComponent Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CPivotComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(Arg)))
    {
        MSG_BOX("PivotComponent Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

CComponent* CPivotComponent::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("PivotComponent Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CPivotComponent::Free()
{
    SUPER::Free();

    Safe_Release(m_pOffsetTransformComp);
}
