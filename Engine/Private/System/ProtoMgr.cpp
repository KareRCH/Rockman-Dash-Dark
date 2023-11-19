#include "System/ProtoMgr.h"

#include "Component/Component.h"

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
    // �����̳� ���� �۾�
    for (auto iter = m_mapProto.begin(); iter != m_mapProto.end();)
    {
        Safe_Release((*iter).second);
        iter = m_mapProto.erase(iter);
    }
}

HRESULT CProtoMgr::Create_Proto(const _tchar* pProtoTag, CComponent* pComponent)
{
    CComponent* pInstance = Find_Prototype(pProtoTag);

    /*if (nullptr != pInstance)
        return E_FAIL;*/
    if (nullptr != pInstance)
    {
        Safe_Release(pInstance);
        auto iter = m_mapProto.find(pProtoTag);
        m_mapProto.erase(iter);
    }

    m_mapProto.emplace(pProtoTag, pComponent);

    return S_OK;
}


CComponent* CProtoMgr::Clone_Proto(const _tchar* pProtoTag)
{
    // ������Ÿ���̵� �ν��Ͻ��� ã�� ������ �ν��Ͻ��� ��ȯ�Ѵ�.
    CComponent* pComponent = Find_Prototype(pProtoTag);

    NULL_CHECK_RETURN(pComponent, nullptr);
    
    return pComponent->Clone();
}

CComponent* CProtoMgr::Clone_Proto(const _tchar* pProtoTag, CComponent*& prComponent)
{
    // ������Ÿ���̵� �ν��Ͻ��� ã�� ������ �ν��Ͻ��� ��ȯ�Ѵ�.
    prComponent = Find_Prototype(pProtoTag);

    NULL_CHECK_RETURN(prComponent, nullptr);
    _int t = 0;
    return prComponent->Clone();
}

CComponent* CProtoMgr::Find_Prototype(const _tchar* pProtoTag)
{
    // ������Ʈ�� ã�´�.
    //auto	iter = find_if(m_mapProto.begin(), m_mapProto.end(), CTag_Finder(pProtoTag));
    auto iter = m_mapProto.find(pProtoTag);

    if (iter == m_mapProto.end())
        return nullptr;

    return iter->second;
}


