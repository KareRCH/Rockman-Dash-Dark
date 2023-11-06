#include "Component/GameObjectComp.h"

CGameObjectComp::CGameObjectComp(const DX11DEVICE_T tDevice)
    : Base(tDevice)
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
