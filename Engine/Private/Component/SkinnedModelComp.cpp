#include "Component/SkinnedModelComp.h"

#include "System/GameInstance.h"

CSkinnedModelComp::CSkinnedModelComp(const CSkinnedModelComp& rhs)
    : Base(rhs)
    , m_pModelData(rhs.m_pModelData)
    , m_pMultiMeshBufComp(rhs.m_pMultiMeshBufComp)
    , m_pSkeletalComp(rhs.m_pSkeletalComp)
    , m_pAnimationComp(rhs.m_pAnimationComp)
    , m_pEffectComp(rhs.m_pEffectComp)
{
    Safe_AddRef(m_pModelData);
    Safe_AddRef(m_pMultiMeshBufComp);
    Safe_AddRef(m_pSkeletalComp);
    Safe_AddRef(m_pAnimationComp);
}

HRESULT CSkinnedModelComp::Initialize_Prototype(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

    m_pMultiMeshBufComp = CMultiMeshBufComp::Create();
    m_pSkeletalComp = CSkeletalComponent::Create();
    m_pAnimationComp = CAnimationComponent::Create();
    m_pEffectComp = CEffectComponent::Create();

    return S_OK;
}

HRESULT CSkinnedModelComp::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

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
    // 여기서 셰이더에 모든 정보를 넘겨주는 행위를 한다.

    MATRIX_BUFFER_T matBuffer = {
        Transform().Get_TransformMatrix(),
        PipelineComp().Get_ViewMatrix(ECamType::Pers, ECamNum::One),
        PipelineComp().Get_ProjMatrix(ECamType::Pers, ECamNum::One)
    };
    LIGHT_BUFFER_T lightBuffer = { 
        _float4(0.2f, 0.2f, 0.2f, 1.f), 
        _float4(0.2f, 0.2f, 0.2f, 1.f), 
        _float3(0.f, -0.2f, 1.f),                
        _float(2.f), _float4(1.f, 0.2f, 0.2f, 1.f)
    };
    BONE_COMMON_BUFFER_T boneBuffer = {};

    _float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
    vector<_float4x4> vecBones = m_pSkeletalComp->Get_FinalTransforms();

    m_pEffectComp->Bind_Matrix("g_matWorld", &matTemp);
    m_pEffectComp->Bind_Matrix("g_matView", &(matTemp = PipelineComp().Get_ViewFloat4x4(ECamType::Pers, ECamNum::One)));
    m_pEffectComp->Bind_Matrix("g_matProj", &(matTemp = PipelineComp().Get_ProjFloat4x4(ECamType::Pers, ECamNum::One)));
    m_pEffectComp->Bind_Matrices("g_matBones", vecBones.data(), (_uint)vecBones.size());
    m_pEffectComp->Bind_RawValue("g_colDiffuse", VPCast(&lightBuffer.vDiffuseColor), sizeof(_float4));
    m_pEffectComp->Bind_RawValue("g_colAmbient", VPCast(&lightBuffer.vAmbientColor), sizeof(_float4));
    m_pEffectComp->Bind_RawValue("g_colSpecular", VPCast(&lightBuffer.vSpecularColor), sizeof(_float4));
    m_pEffectComp->Bind_RawValue("g_fSpecularPower", VPCast(&lightBuffer.fSpecularPower), sizeof(_float));
    m_pEffectComp->Bind_RawValue("g_vLightDir", VPCast(&lightBuffer.vLightDirection), sizeof(_float3));
    m_pEffectComp->Bind_SRV("g_texDiffuse", GI()->Find_SRV(L"Model/Character/RockVolnutt/Body.png"));

    // 그리기 시작
    m_pEffectComp->Begin(0);

    for (_uint i = 0; i < m_pMultiMeshBufComp->Get_MeshesCounts(); ++i)
    {
        // 버퍼를 장치에 바인드
        m_pMultiMeshBufComp->Bind_Buffer(i);

        // 바인딩된 정점, 인덱스 그리기
        m_pMultiMeshBufComp->Render_Buffer(i);
    }
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

    Safe_Release(m_pModelData);
    Safe_Release(m_pMultiMeshBufComp);
    Safe_Release(m_pSkeletalComp);
    Safe_Release(m_pAnimationComp);
    Safe_Release(m_pEffectComp);
}

HRESULT CSkinnedModelComp::Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey)
{
    if (!m_pMultiMeshBufComp
        || !m_pSkeletalComp
        || !m_pAnimationComp)
        return E_FAIL;

    m_pModelData = ConCast<FModelData*>(GI()->Find_ModelData(eGroup, strModelKey));
    Safe_AddRef(m_pModelData);

    m_pMultiMeshBufComp->Set_ModelData(m_pModelData);
    m_pSkeletalComp->Set_ModelData(m_pModelData);
    m_pAnimationComp->Set_ModelData(m_pModelData);

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
    if (!m_pSkeletalComp)
        return E_FAIL;

    return m_pSkeletalComp->Load_Skeletal(strSkeletalKey);
}


