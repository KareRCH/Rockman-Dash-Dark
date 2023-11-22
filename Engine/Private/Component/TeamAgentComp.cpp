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

_int CTeamAgentComp::Tick(const _float& fTimeDelta)
{
    return _int();
}

void CTeamAgentComp::Late_Tick(const _float& fTimeDelta)
{
}

void CTeamAgentComp::Render()
{
}

CTeamAgentComp* CTeamAgentComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Engine::Safe_Release(pInstance);
        MSG_BOX("CTeamAgentComp Create Failed");

        return nullptr;
    }

    return pInstance;
}

CComponent* CTeamAgentComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);
        MSG_BOX("CSkeletonComponent Create Failed");

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CTeamAgentComp::Free()
{
    SUPER::Free();
}
