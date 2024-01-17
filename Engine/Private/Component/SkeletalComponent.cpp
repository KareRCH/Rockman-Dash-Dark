#include "Component/SkeletalComponent.h"

#include "System/ModelMgr.h"
#include "System/GameInstance.h"
#include "System/Data/BoneData.h"

CSkeletalComponent::CSkeletalComponent(const CSkeletalComponent& rhs)
    : Base(rhs)
{
    m_pBoneGroup = rhs.m_pBoneGroup->Clone();
}

HRESULT CSkeletalComponent::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CSkeletalComponent::Initialize(void* Arg)
{
    return S_OK;
}

CSkeletalComponent* CSkeletalComponent::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CSkeletonComponent Create Failed");
        Safe_Release(pInstance);
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
    }

    return Cast<CComponent*>(pInstance);
}

void CSkeletalComponent::Free()
{
    SUPER::Free();

    Safe_Release(m_pBoneGroup);
}

HRESULT CSkeletalComponent::Load_Skeletal(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath)
{
    if (nullptr == m_pGI)
        return E_FAIL;

    CBoneGroup* pBoneGroup = m_pGI->Clone_BoneGroup(eGroupIndex, strModelFilePath);
    if (nullptr == pBoneGroup)
        return E_FAIL;

    m_pBoneGroup = pBoneGroup;

    return S_OK;
}

CBoneGroup* CSkeletalComponent::Get_BoneGroup()
{
    return m_pBoneGroup;
}

void CSkeletalComponent::Invalidate_BoneTransforms()
{
    if (!m_pBoneGroup)
        return;

    m_pBoneGroup->Invalidate_FinalTransforms();
}
