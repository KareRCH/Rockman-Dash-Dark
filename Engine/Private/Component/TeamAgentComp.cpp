#include "Component/TeamAgentComp.h"

_uint_64						            CTeamAgentComp::iID_Count = { 0 };
map<_ubyte, CTeamAgentComp::SRelationMap>	CTeamAgentComp::m_mapTeamRelation;

CTeamAgentComp::CTeamAgentComp()
    : Base()
    , m_iPrivID(InitID())
{
    
}

CTeamAgentComp::CTeamAgentComp(const CTeamAgentComp& rhs)
    : Base(rhs)
    , m_iTeamID(rhs.m_iTeamID)
    , m_iPrivID(rhs.m_iPrivID)
{
}

HRESULT CTeamAgentComp::Initialize_Prototype(void* Arg)
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTeamAgentComp::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTeamAgentComp::Initialize(void* Arg)
{

    return S_OK;
}

HRESULT CTeamAgentComp::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

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

FSerialData CTeamAgentComp::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

FSerialData CTeamAgentComp::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
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

CTeamAgentComp* CTeamAgentComp::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
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

CComponent* CTeamAgentComp::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
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
