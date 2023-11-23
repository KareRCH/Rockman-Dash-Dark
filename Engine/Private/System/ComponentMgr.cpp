#include "System/ComponentMgr.h"

#include "Component/Component.h"

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
    // ������Ÿ�� ����
    for (auto& Pair : m_mapPrototypes)
        Safe_Release(Pair.second);
    m_mapPrototypes.clear();
}

HRESULT CComponentMgr::Add_Prototype(const wstring& strProtoKey, CComponent* pPrototype)
{
    if (nullptr == pPrototype)
        return E_FAIL;

    CComponent* pInstance = Find_Prototype(strProtoKey);
    
    if (nullptr != pInstance)
    {
        Safe_Release(pPrototype);
        MSG_BOX("Add Component Proto Failed");
        return E_FAIL;
    }

    m_mapPrototypes.emplace(strProtoKey, pPrototype);

    return S_OK;
}


CComponent* CComponentMgr::Clone_Prototype(const wstring& strProtoKey, void* pArg)
{
    // ������Ÿ���̵� �ν��Ͻ��� ã�� ������ �ν��Ͻ��� ��ȯ�Ѵ�.
    CComponent* pPrototype = Find_Prototype(strProtoKey);

    if (nullptr == pPrototype)
        return nullptr;

    CComponent* pComponent = pPrototype->Clone(pArg);

    if (nullptr == pComponent)
        return nullptr;
    
    return Cast<CComponent*>(pComponent);
}

void CComponentMgr::Clear_Prototypes(const wstring& strContainTag)
{
    // �з����� ������Ÿ���� �޸� �����ϴ� �۾�
    for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
    {
        CComponent** ppComp = &(*iter).second;
        if ((*ppComp)->Has_LevelTag(strContainTag))
        {
            // �±� ������ �ٸ� �±װ� ������ �������� �ʴ´�. (�ٸ� �������� ��������)
            (*ppComp)->Delete_LevelTag(strContainTag);
            if ((*ppComp)->LevelTag_Size() == 0U)
            {
                Safe_Release(*ppComp);
                iter = m_mapPrototypes.erase(iter);
            }
            else
                ++iter;
        }
        else
            ++iter;
    }
}

CComponent* CComponentMgr::Find_Prototype(const wstring& strProtoKey)
{
    // ������Ʈ�� ã�´�.
    auto iter = m_mapPrototypes.find(strProtoKey);
    if (iter == m_mapPrototypes.end())
        return nullptr;

    return iter->second;
}


