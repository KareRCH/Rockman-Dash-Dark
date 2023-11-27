#include "Component/CloudStationComp.h"

CCloudStationComp::CCloudStationComp(const CCloudStationComp& rhs)
	: Base(rhs)
{
}

HRESULT CCloudStationComp::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CCloudStationComp::Initialize(void* Arg)
{
    return S_OK;
}

void CCloudStationComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CCloudStationComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CCloudStationComp::Late_Tick(const _float& fTimeDelta)
{
}

void CCloudStationComp::Render()
{
}

CCloudStationComp* CCloudStationComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("BoxBufferComp Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CCloudStationComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("BoxBufferComp Copy Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CCloudStationComp::Free()
{
}
