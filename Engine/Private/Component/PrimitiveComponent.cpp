#include "Component/PrimitiveComponent.h"

CPrimitiveComponent::CPrimitiveComponent(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_bClone(false)
{
}

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: m_pDevice(rhs.m_pDevice), m_pDeviceContext(rhs.m_pDeviceContext), m_bClone(true)
{
}

void CPrimitiveComponent::Free()
{
	SUPER::Free();

	for (auto& Pair : m_mapPrimComponent)
	{
		Safe_Release(Pair.second);
	}
	m_mapPrimComponent.clear();
}

HRESULT CPrimitiveComponent::Add_PrimComponent(const wstring& strCompKey, CPrimitiveComponent* pComp)
{
	auto iter = m_mapPrimComponent.find(strCompKey);
	if (iter != m_mapPrimComponent.end())
	{
		Safe_Release(pComp);

		MSG_BOX("Add_PrimComp Failed!");

		return E_FAIL;
	}

	m_mapPrimComponent.emplace(strCompKey, pComp);

	return S_OK;
}

CPrimitiveComponent* CPrimitiveComponent::Find_PrimComponent(const wstring& strCompKey)
{
	auto iter = m_mapPrimComponent.find(strCompKey);
	if (iter == m_mapPrimComponent.end())
		return nullptr;
	
	return (*iter).second;
}


