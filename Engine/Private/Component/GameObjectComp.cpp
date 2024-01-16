#include "Component/GameObjectComp.h"

#include "System/GameInstance.h"

CGameObjectComp::CGameObjectComp()
{
    NULL_CHECK(m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp")));
}

CGameObjectComp::CGameObjectComp(const CGameObjectComp& rhs)
    : Base(rhs)
    , m_pDeviceComp(rhs.m_pDeviceComp)
{
    Safe_AddRef(m_pDeviceComp);
}

HRESULT CGameObjectComp::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CGameObjectComp::Initialize_Prototype(FSerialData& InputData)
{
    string strPrototypeName = "";
    InputData.Get_Data("ProtoName", strPrototypeName);
    m_strPrototypeName = ConvertToWstring(strPrototypeName);

    return S_OK;
}

HRESULT CGameObjectComp::Initialize(FSerialData& InputData)
{
    string strPrototypeName = "";
    InputData.Get_Data("ProtoName", strPrototypeName);
    m_strPrototypeName = ConvertToWstring(strPrototypeName);

    return S_OK;
}

FSerialData CGameObjectComp::SerializeData_Prototype()
{
    FSerialData Data;

    Data.Add_Member("ComponentID", 0U);
    Data.Add_MemberString("ProtoName", ConvertToString(m_strPrototypeName));

    return Data;
}

FSerialData CGameObjectComp::SerializeData()
{
    FSerialData Data;

    Data.Add_Member("ComponentID", 0U);
    Data.Add_MemberString("ProtoName", ConvertToString(m_strPrototypeName));

    return Data;
}

void CGameObjectComp::Free()
{
    SUPER::Free();

    for (auto& Pair : m_mapPrimComponent)
        Safe_Release(Pair.second);
    m_mapPrimComponent.clear();

    Safe_Release(m_pDeviceComp);
}

HRESULT CGameObjectComp::Add_PrimComponent(const wstring& strCompKey, CGameObjectComp* pComp)
{
    auto iter = m_mapPrimComponent.find(strCompKey);
    if (iter != m_mapPrimComponent.end())
    {
        MSG_BOX("Add_PrimComp Failed!");
        Safe_Release(pComp);

        return E_FAIL;
    }

    m_mapPrimComponent.emplace(strCompKey, pComp);

    return S_OK;
}

CGameObjectComp* CGameObjectComp::Find_PrimComponent(const wstring& strCompKey)
{
    auto iter = m_mapPrimComponent.find(strCompKey);
    if (iter == m_mapPrimComponent.end())
        return nullptr;

    return (*iter).second;
}
