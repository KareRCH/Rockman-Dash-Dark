#include "Component/GameObjectComp.h"

CGameObjectComp::CGameObjectComp(const CGameObjectComp& rhs)
    : Base(rhs)
{
}

void CGameObjectComp::Free()
{
    SUPER::Free();
}
