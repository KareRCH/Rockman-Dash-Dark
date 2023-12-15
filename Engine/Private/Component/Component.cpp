#include "Component/Component.h"

#include "System/GameInstance.h"

CComponent::CComponent()
	: m_bClone(true)
	, m_pGameInst(GI())
{
}

CComponent::CComponent(const CComponent& rhs)
	: m_bClone(false)
{
}

void CComponent::Delete_LevelTag(const wstring& strTag)
{
	auto iter = m_setLevelTag.find(strTag);

	if (iter != m_setLevelTag.end())
		m_setLevelTag.erase(iter);
}
