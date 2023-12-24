#include "Component/Data/NaviRegion.h"

CNaviRegion::CNaviRegion()
{
}

CNaviRegion::CNaviRegion(const CNaviRegion& rhs)
{
}

HRESULT CNaviRegion::Initialize(const BoundingBox& AABB, _uint& iIndex)
{
    return S_OK;
}

HRESULT CNaviRegion::Render(CEffectComponent* pEffectComp)
{
    return S_OK;
}

CNaviRegion* CNaviRegion::Create(const BoundingBox AABB, _uint iIndex)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(AABB, iIndex)))
    {
        MSG_BOX("BoxBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CNaviRegion::Free()
{
}

_bool CNaviRegion::IsIn(_fvector vPosition, _int* pNeighborIndex)
{
    for (_uint i = 0; i < DIR_END; i++)
    {
        if (m_iNeighbors[i] == -1)
            continue;

        //m_AABB.Intersects();
    }

    return _bool();
}
