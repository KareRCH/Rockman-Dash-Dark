#include "Component/Component.h"

CComponent::CComponent()
{
}

CComponent::CComponent(const CComponent& rhs)
{
}

void CComponent::Free()
{
	for (auto& Pair : m_mapInterComponent)
	{
		Safe_Release(Pair.second);
	}
	m_mapInterComponent.clear();
}

HRESULT CComponent::Add_InterComponent(const wstring& strCompKey, CComponent* pComp)
{
	auto iter = m_mapInterComponent.find(strCompKey);
	if (iter != m_mapInterComponent.end())
	{
		Safe_Release(pComp);

		MSG_BOX("Add_Comp Failed!");

		return E_FAIL;
	}

	m_mapInterComponent.emplace(strCompKey, pComp);

	return S_OK;
}

CComponent* CComponent::Find_InterComponent(const wstring& strCompKey)
{
	auto iter = m_mapInterComponent.find(strCompKey);
	if (iter == m_mapInterComponent.end())
		return nullptr;

	return (*iter).second;
}
