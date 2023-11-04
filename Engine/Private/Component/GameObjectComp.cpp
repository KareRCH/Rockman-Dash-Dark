#include "Component/GameObjectComp.h"

CGameObjectComp::CGameObjectComp(ID3D11Device* pDevice)
    : Base(pDevice)
{
}

CGameObjectComp::CGameObjectComp(const CGameObjectComp& rhs)
    : Base(rhs)
{
}

void CGameObjectComp::Free()
{
    SUPER::Free();
}
