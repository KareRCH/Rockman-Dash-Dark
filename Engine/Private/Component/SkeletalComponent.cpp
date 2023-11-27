#include "Component/SkeletalComponent.h"

#include "System/ModelMgr.h"
#include "System/GameInstance.h"

CSkeletalComponent::CSkeletalComponent(const CSkeletalComponent& rhs)
    : Base(rhs)
{
}

HRESULT CSkeletalComponent::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CSkeletalComponent::Initialize(void* Arg)
{
    FInitData tInit = {};
    
    if (nullptr != Arg)
        tInit = (*ReCast<FInitData*>(Arg));

    return S_OK;
}

CSkeletalComponent* CSkeletalComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CSkeletonComponent Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CSkeletalComponent::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CSkeletonComponent Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CSkeletalComponent::Free()
{
    Safe_Release(m_pArmature);
}

HRESULT CSkeletalComponent::Load_Armature(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strArmatureKey)
{
    FArmatureData* pArmatureData = GI()->Find_Armature(eGroupIndex, strModelKey, strArmatureKey);

    if (!pArmatureData)
        return E_FAIL;
    
    m_pArmature = pArmatureData;
    Safe_AddRef(m_pArmature);

    return S_OK;
}
