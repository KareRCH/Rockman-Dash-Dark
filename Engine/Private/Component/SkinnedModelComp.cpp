#include "Component/SkinnedModelComp.h"

#include "System/GameInstance.h"

CSkinnedModelComp::CSkinnedModelComp()
{
    m_pMultiMeshBufComp = CMultiMeshBufComp::Create();
    m_pSkeletalComp = CSkeletalComponent::Create();
    m_pAnimationComp = CAnimationComponent::Create();
    m_pEffectComp = CEffectComponent::Create();
}

CSkinnedModelComp::CSkinnedModelComp(const CSkinnedModelComp& rhs)
    : Base(rhs)
    , m_pModelData(rhs.m_pModelData)
    , m_pMultiMeshBufComp(rhs.m_pMultiMeshBufComp)
    , m_pSkeletalComp(rhs.m_pSkeletalComp)
    , m_pEffectComp(rhs.m_pEffectComp)
{
    Safe_AddRef(m_pModelData);
    Safe_AddRef(m_pMultiMeshBufComp);
    Safe_AddRef(m_pSkeletalComp);
    m_pAnimationComp = Cast<CAnimationComponent*>(rhs.m_pAnimationComp->Clone());
}

HRESULT CSkinnedModelComp::Initialize_Prototype(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);

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

void CSkinnedModelComp::Tick(const _float& fTimeDelta)
{

}

void CSkinnedModelComp::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSkinnedModelComp::Render()
{
    // 여기서 셰이더에 모든 정보를 넘겨주는 행위를 한다.

    LIGHT_BUFFER_T lightBuffer = { 
        _float4(0.2f, 0.2f, 0.2f, 1.f), 
        _float4(0.2f, 0.2f, 0.2f, 1.f), 
        _float3(0.f, -0.2f, 1.f),                
        _float(50.f), _float4(1.f, 0.2f, 0.2f, 1.f)
    };

    
    for (_uint i = 0; i < m_pMultiMeshBufComp->Get_MeshesCounts(); ++i)
    {
        _float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
        //_float4x4 matMesh = m_pMultiMeshBufComp->Get_MeshTransform(i);
        //XMStoreFloat4x4(&matTemp, XMLoadFloat4x4(&matTemp) * XMLoadFloat4x4(&matMesh));
        
        m_pEffectComp->Bind_Matrix("g_matWorld", &matTemp);
        m_pEffectComp->Bind_Matrix("g_matView", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
        m_pEffectComp->Bind_Matrix("g_matProj", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));
        m_pMultiMeshBufComp->Bind_MeshOffsetsToEffect(m_pEffectComp, "g_matBones", i, *m_pSkeletalComp->Get_BoneGroup());

        m_pEffectComp->Bind_RawValue("g_colDiffuse", VPCast(&lightBuffer.vDiffuseColor), sizeof(_float4));
        m_pEffectComp->Bind_RawValue("g_colAmbient", VPCast(&lightBuffer.vAmbientColor), sizeof(_float4));
        m_pEffectComp->Bind_RawValue("g_colSpecular", VPCast(&lightBuffer.vSpecularColor), sizeof(_float4));
        m_pEffectComp->Bind_RawValue("g_fSpecularPower", VPCast(&lightBuffer.fSpecularPower), sizeof(_float));
        m_pEffectComp->Bind_RawValue("g_vLightDir", VPCast(&lightBuffer.vLightDirection), sizeof(_float3));

        m_pEffectComp->Bind_SRV("g_texDiffuse", GI()->Find_SRV(L"Model/Character/Megaman/Image_0.png"));

        // 그리기 시작
        m_pEffectComp->Begin(0);

        // 버퍼를 장치에 바인드
        m_pMultiMeshBufComp->Bind_Buffer(i);

        // 바인딩된 정점, 인덱스 그리기
        m_pMultiMeshBufComp->Render_Buffer(i);
    }

    return S_OK;
}

CSkinnedModelComp* CSkinnedModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("SkinnedModelComp Create Failed");
        Safe_Release(pInstance);
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

HRESULT CSkinnedModelComp::Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    if (!m_pEffectComp)
        return E_FAIL;

    return m_pEffectComp->Bind_Effect(strEffectKey, pElements, iNumElements);
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
    m_pSkeletalComp->Load_Skeletal(strSkeletalKey);

    return m_pAnimationComp->Bind_BoneGroup(m_pSkeletalComp);
}

void CSkinnedModelComp::Invalidate_BoneTransforms()
{
    if (!m_pSkeletalComp)
        return;

    return m_pSkeletalComp->Invalidate_BoneTransforms();
}

HRESULT CSkinnedModelComp::Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive)
{
    if (!m_pAnimationComp)
        return E_FAIL;

    return m_pAnimationComp->Create_Mask(strMaskName, strSkeletalName, bInitBoneActive);
}

void CSkinnedModelComp::Deactive_BoneMask(_uint iIndex, const wstring& strBoneName)
{
    if (!m_pAnimationComp)
        return;

    m_pAnimationComp->Deactive_BoneMask(iIndex, strBoneName);
}

void CSkinnedModelComp::Active_BoneMask(_uint iIndex, const wstring& strBoneName)
{
    if (!m_pAnimationComp)
        return;

    m_pAnimationComp->Active_BoneMask(iIndex, strBoneName);
}

void CSkinnedModelComp::Set_MaskAnimation(_uint iIndex, const wstring& strAnimName)
{
    if (!m_pAnimationComp)
        return;

    m_pAnimationComp->Set_MaskAnimation(iIndex, strAnimName);
}

void CSkinnedModelComp::Set_MaskTime(_uint iIndex, _float fTime)
{
    if (!m_pAnimationComp)
        return;

    m_pAnimationComp->Set_MaskTime(iIndex, fTime);
}

HRESULT CSkinnedModelComp::Apply_Pose()
{
    if (!m_pAnimationComp)
        return E_FAIL;

    m_pAnimationComp->Apply_FinalMask();

    return S_OK;
}


