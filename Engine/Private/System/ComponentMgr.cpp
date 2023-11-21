#include "System/ComponentMgr.h"

#include "Component/Component.h"

CComponentMgr::CComponentMgr()
{
}

HRESULT CComponentMgr::Initialize()
{
    return S_OK;
}

CComponentMgr* CComponentMgr::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ProtoMgr Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CComponentMgr::Free()
{
    // 컨테이너 해제 작업
    for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
    {
        Safe_Release((*iter).second);
        iter = m_mapPrototypes.erase(iter);
    }
}

HRESULT CComponentMgr::Create_Proto(const wstring& strProtoKey, CComponent* pComponent)
{
    CComponent* pInstance = Find_Prototype(strProtoKey);

    /*if (nullptr != pInstance)
        return E_FAIL;*/
    if (nullptr != pInstance)
    {
        Safe_Release(pInstance);
        auto iter = m_mapPrototypes.find(strProtoKey);
        m_mapPrototypes.erase(iter);
    }

    m_mapPrototypes.emplace(strProtoKey, pComponent);

    return S_OK;
}


CComponent* CComponentMgr::Clone_Proto(const wstring& strProtoKey)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    CComponent* pComponent = Find_Prototype(strProtoKey);

    NULL_CHECK_RETURN(pComponent, nullptr);
    
    return pComponent->Clone();
}

CComponent* CComponentMgr::Clone_Proto(const wstring& strProtoKey, CComponent*& prComponent)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    prComponent = Find_Prototype(strProtoKey);

    NULL_CHECK_RETURN(prComponent, nullptr);
    _int t = 0;
    return prComponent->Clone();
}

CComponent* CComponentMgr::Find_Prototype(const wstring& strProtoKey)
{
    // 컴포넌트를 찾는다.
    //auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));
    auto iter = m_mapPrototypes.find(strProtoKey);

    if (iter == m_mapPrototypes.end())
        return nullptr;

    return iter->second;
}


