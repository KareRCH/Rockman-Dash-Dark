#include "System/ComponentMgr.h"

#include "Component/Component.h"
#include "Utility/RapidJsonSerial.h"

HRESULT CComponentMgr::Initialize()
{
    return S_OK;
}

CComponentMgr* CComponentMgr::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ProtoMgr Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

void CComponentMgr::Free()
{
    // 프로토타입 해제
    for (auto& Pair : m_mapPrototypes)
        Safe_Release(Pair.second);
    m_mapPrototypes.clear();
}

HRESULT CComponentMgr::Add_Prototype(const wstring& strTag, const wstring& strProtoKey, CComponent* pPrototype)
{
    if (nullptr == pPrototype)
        return E_FAIL;

    CComponent* pInstance = Find_Prototype(strProtoKey);
    if (nullptr != pInstance)
    {
        Safe_Release(pPrototype);
        return E_FAIL;
    }

    pPrototype->Add_LevelTag(strTag);
    m_mapPrototypes.emplace(strProtoKey, pPrototype);

    return S_OK;
}


CComponent* CComponentMgr::Clone_Prototype(const wstring& strProtoKey, void* pArg)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    CComponent* pPrototype = Find_Prototype(strProtoKey);

    if (nullptr == pPrototype)
        return nullptr;

    CComponent* pComponent = pPrototype->Clone(pArg);

    if (nullptr == pComponent)
        return nullptr;
    
    return pComponent;
}

CComponent* CComponentMgr::Clone_Prototype(const wstring& strProtoKey, FSerialData& InputData)
{
    // 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
    CComponent* pPrototype = Find_Prototype(strProtoKey);

    if (nullptr == pPrototype)
        return nullptr;

    CComponent* pComponent = pPrototype->Clone(InputData);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent;
}

CComponent* CComponentMgr::Reference_Prototype(const wstring& strProtoKey)
{
    // 프로토타입을 참조하는 형태로 사용할 경우 이 함수를 이용한다.
    CComponent* pPrototype = Find_Prototype(strProtoKey);

    if (nullptr == pPrototype)
        return nullptr;

    Safe_AddRef(pPrototype);

    return pPrototype;
}

void CComponentMgr::Clear_Prototypes(const wstring& strContainTag)
{
    // 분류별로 프로토타입을 메모리 해제하는 작업
    for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
    {
        CComponent** ppComp = &(*iter).second;
        if ((*ppComp)->Has_LevelTag(strContainTag))
        {
            // 태그 삭제후 다른 태그가 있으면 삭제하지 않는다. (다른 레벨에도 속해있음)
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
    // 컴포넌트를 찾는다.
    auto iter = m_mapPrototypes.find(strProtoKey);
    if (iter == m_mapPrototypes.end())
        return nullptr;

    return Cast<CComponent*>(iter->second);
}


