#include "Component/PrimitiveComponent.h"

#include "Component/D3D11DeviceComp.h"

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: Base(rhs)
	, m_pDeviceComp(rhs.m_pDeviceComp)
{
	Safe_AddRef(m_pDeviceComp);
}

HRESULT CPrimitiveComponent::Initialize_Prototype(void* Arg)
{
	m_pDeviceComp = CD3D11DeviceComp::Create();

	return S_OK;
}

void CPrimitiveComponent::Free()
{
	for (auto& Pair : m_mapPrimComponent)
		Safe_Release(Pair.second);
	m_mapPrimComponent.clear();
	Safe_Release(m_pDeviceComp);
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


