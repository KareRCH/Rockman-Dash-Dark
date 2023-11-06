#include "System/ProtoMgr.h"

#include "Component/PrimitiveComponent.h"

CProtoMgr::CProtoMgr()
{
}

HRESULT CProtoMgr::Initialize()
{
    return S_OK;
}

CProtoMgr* CProtoMgr::Create()
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

void CProtoMgr::Free()
{
    // 컨테이너 해제 작업
    for (auto iter = m_mapProto.begin(); iter != m_mapProto.end();)
    {
        Safe_Release((*iter).second);
        iter = m_mapProto.erase(iter);
    }
}

HRESULT CProtoMgr::Create_Proto(const _tchar* pProtoTag, CPrimitiveComponent* pComponent)
{
    CPrimitiveComponent* pInstance = Find_Prototype(pProtoTag);

    /*if (nullptr != pInstance)
        return E_FAIL;*/
    if (nullptr != pInstance)
    {
        Safe_Release(pInstance);
        auto iter = m_mapProto.find(pProtoTag);
        m_mapProto.erase(iter);
    }

    m_mapProto.insert({ pProtoTag, pComponent });

    return S_OK;
}


CPrimitiveComponent* CProtoMgr::Clone_Proto(const _tchar* pProtoTag)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    CPrimitiveComponent* pComponent = Find_Prototype(pProtoTag);

    NULL_CHECK_RETURN(pComponent, nullptr);
    _int t = 0;
    return pComponent->Clone(&t);
}

CPrimitiveComponent* CProtoMgr::Clone_Proto(const _tchar* pProtoTag, CPrimitiveComponent*& prComponent)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    prComponent = Find_Prototype(pProtoTag);

    NULL_CHECK_RETURN(prComponent, nullptr);
    _int t = 0;
    return prComponent->Clone(&t);
}

CPrimitiveComponent* CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
    // 컴포넌트를 찾는다.
    //auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));
    auto iter = m_mapProto.find(pProtoTag);

    if (iter == m_mapProto.end())
        return nullptr;

    return iter->second;
}


