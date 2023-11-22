#include "BaseClass/Terrain.h"

CTerrain::CTerrain(const CTerrain& rhs)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::Initialize(void* Arg)
{
    return S_OK;
}

void CTerrain::Priority_Tick(const _float& fTimeDelta)
{
}

_int CTerrain::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CTerrain::Late_Tick(const _float& fTimeDelta)
{
}

void CTerrain::Render()
{
}

void CTerrain::Free()
{
}
