#include "Component/Component.h"


CComponent::CComponent()
	: m_bIsCloned(false)
{
	NULL_CHECK(m_pGI = GI());
	Safe_AddRef(m_pGI);
}

CComponent::CComponent(const CComponent& rhs)
	: m_bIsCloned(true)
{
	NULL_CHECK(m_pGI = rhs.m_pGI);
	Safe_AddRef(m_pGI);
}

void CComponent::Free()
{
	Safe_Release(m_pGI);
}

void CComponent::Delete_LevelTag(const wstring& strTag)
{
	auto iter = m_setLevelTag.find(strTag);

	if (iter != m_setLevelTag.end())
		m_setLevelTag.erase(iter);
}
