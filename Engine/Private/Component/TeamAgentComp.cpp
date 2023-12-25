#include "Component/TeamAgentComp.h"

CTeamAgentComp::CTeamAgentComp(const CTeamAgentComp& rhs)
{
}

HRESULT CTeamAgentComp::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CTeamAgentComp::Initialize(void* Arg)
{
    return S_OK;
}

void CTeamAgentComp::Priority_Tick(const _float& fTimeDelta)
{
}

void CTeamAgentComp::Tick(const _float& fTimeDelta)
{
    
}

void CTeamAgentComp::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CTeamAgentComp::Render()
{
    return S_OK;
}

CTeamAgentComp* CTeamAgentComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("TeamAgentComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CTeamAgentComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TeamAgentComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CTeamAgentComp::Free()
{
    SUPER::Free();
}
