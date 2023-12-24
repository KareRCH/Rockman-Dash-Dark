#include "BaseClass/Level.h"

#include "System/GameInstance.h"

CLevel::CLevel()
{
	NULL_CHECK(m_pGI = GI());
	Safe_AddRef(m_pGI);
}

void CLevel::Free()
{
	Safe_Release(m_pGI);
}
