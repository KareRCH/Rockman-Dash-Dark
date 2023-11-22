#include "Component/SkeletonComponent.h"

#include "System/ModelMgr.h"
#include "System/GameInstance.h"

CSkeletonComponent::CSkeletonComponent(const CSkeletonComponent& rhs)
    : Base(rhs)
{
}

HRESULT CSkeletonComponent::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CSkeletonComponent::Initialize(void* Arg)
{
    FInitialData tInit = {};
    
    if (nullptr != Arg)
        tInit = (*ReCast<FInitialData*>(Arg));

    return S_OK;
}

CSkeletonComponent* CSkeletonComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        Engine::Safe_Release(pInstance);
        MSG_BOX("CSkeletonComponent Create Failed");

        return nullptr;
    }

    return pInstance;
}

CComponent* CSkeletonComponent::Clone(void* Arg)
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

void CSkeletonComponent::Free()
{
    
}

HRESULT CSkeletonComponent::Load_BoneRootNode(const EModelGroupIndex eGroupIndex, const wstring& strModelKey, const wstring& strArmatureKey)
{
    FArmatureData* pArmatureData = GI()->Clone_Armature(eGroupIndex, strModelKey, strArmatureKey);

    return S_OK;
}
