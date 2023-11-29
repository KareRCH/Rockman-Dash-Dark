#include "Component/SkinnedModelComp.h"

#include "System/GameInstance.h"

CSkinnedModelComp::CSkinnedModelComp(const CSkinnedModelComp& rhs)
    : Base(rhs)
    , m_pModelData(rhs.m_pModelData)
    , m_pMultiMeshBufComp(rhs.m_pMultiMeshBufComp)
    , m_pSkeletalComponent(rhs.m_pSkeletalComponent)
    , m_pAnimationComponent(rhs.m_pAnimationComponent)
    , m_pEffectComp(rhs.m_pEffectComp)
{
}

HRESULT CSkinnedModelComp::Initialize_Prototype(void* Arg)
{


    return S_OK;
}

HRESULT CSkinnedModelComp::Initialize(void* Arg)
{


    return S_OK;
}

void CSkinnedModelComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CSkinnedModelComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CSkinnedModelComp::Late_Tick(const _float& fTimeDelta)
{
}

void CSkinnedModelComp::Render()
{
    // ���⼭ 


}

CSkinnedModelComp* CSkinnedModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return pInstance;
}

CComponent* CSkinnedModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CSkinnedModelComp::Free()
{
    SUPER::Free();

}

HRESULT CSkinnedModelComp::Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey)
{
    if (!m_pMultiMeshBufComp
        || !m_pSkeletalComponent
        || !m_pAnimationComponent)
        return E_FAIL;

    m_pModelData = ConCast<FModelData*>(GI()->Find_ModelData(eGroup, strModelKey));
    Safe_AddRef(m_pModelData);

    m_pMultiMeshBufComp->Set_ModelData(m_pModelData);
    m_pSkeletalComponent->Set_ModelData(m_pModelData);
    m_pAnimationComponent->Set_ModelData(m_pModelData);

    return S_OK;
}

HRESULT CSkinnedModelComp::Bind_Mesh(const wstring& strMeshKey)
{
    if (!m_pMultiMeshBufComp)
        return E_FAIL;

    return m_pMultiMeshBufComp->Bind_Mesh(strMeshKey);
}

HRESULT CSkinnedModelComp::Unbind_Mesh(const wstring& strMeshKey)
{
    if (!m_pMultiMeshBufComp)
        return E_FAIL;

    return m_pMultiMeshBufComp->Unbind_Mesh(strMeshKey);
}

void CSkinnedModelComp::Unbind_AllMeshes()
{
    if (!m_pMultiMeshBufComp)
        return;

    m_pMultiMeshBufComp->Unbind_AllMeshes();
}

HRESULT CSkinnedModelComp::Bind_Effect(const wstring& strEffectKey)
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Bind_Effect(strEffectKey);
}

HRESULT CSkinnedModelComp::Unbind_Effect()
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Unbind_Effect();
}

HRESULT CSkinnedModelComp::Bind_Skeletal(const wstring& strSkeletalKey)
{
    if (!m_pSkeletalComponent)
        return E_FAIL;

    return m_pSkeletalComponent->Load_Skeletal(strSkeletalKey);
}


