#include "Component/SkeletalComponent.h"

#include "System/ModelMgr.h"
#include "System/GameInstance.h"

CSkeletalComponent::CSkeletalComponent(const CSkeletalComponent& rhs)
    : Base(rhs)
    , m_pModelData(rhs.m_pModelData)
    , m_pSkeletalData(rhs.m_pSkeletalData)
{
    Safe_AddRef(m_pModelData);
    Safe_AddRef(m_pSkeletalData);
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
    Safe_Release(m_pModelData);
    Safe_Release(m_pSkeletalData);
}

void CSkeletalComponent::Set_ModelData(FModelData* pModelData)
{
    m_pModelData = pModelData;
    Safe_AddRef(m_pModelData);
}

HRESULT CSkeletalComponent::Load_Skeletal(const wstring& strSkeletalKey)
{
    if (!m_pModelData)
        return E_FAIL;

    FSkeletalData* pSkeletalData = m_pModelData->pBoneGroup->Find_SkeletalData(strSkeletalKey);

    if (!pSkeletalData)
        return E_FAIL;
    
    m_pSkeletalData = pSkeletalData;
    Safe_AddRef(m_pSkeletalData);

    return S_OK;
}

vector<_float4x4> CSkeletalComponent::Get_FinalTransforms(_bool bNoHierarchi)
{
    if (!m_pSkeletalData)
        return vector<_float4x4>();

    return m_pSkeletalData->Provide_FinalTransforms(bNoHierarchi);
}
