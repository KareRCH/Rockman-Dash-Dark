#include "Component/PlaneModelComp.h"

#include "Component/RectBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"



CPlaneModelComp::CPlaneModelComp()
{
    // InternalComponent는 생성시 자동 포함 하도록 설계된다.
    // 이들은 반드시 생성되어야 한다는 규칙을 가진다.
    // 이들에 대한 접근은 reference로 이루어진다.
    NULL_CHECK(m_pVIBufferComp = CRectBufferComp::Create());
    NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
    NULL_CHECK(m_pTextureComp = CTextureComponent::Create());
}

CPlaneModelComp::CPlaneModelComp(const CPlaneModelComp& rhs)
    : Base(rhs)
{
    // InternalComponent 중 얕은 복사, 혹은 깊은 복사가 필요한 경우 여기서 처리하도록 한다.
    // 얕은 복사는 주로 Device, Pipeline과 같은 시스템 접근 컴포넌트가 해당된다.
    // 나머지는 깊은 복사로 진행된다. (버퍼, 셰이더, 텍스처와 같이 공유되는 리소스를 가진 컴포넌트들은
    // 내부적으로 얕은 복사가 일어난다.
    NULL_CHECK(m_pVIBufferComp = Cast<CRectBufferComp*>(rhs.m_pVIBufferComp->Clone()));
    NULL_CHECK(m_pEffectComp = Cast<CEffectComponent*>(rhs.m_pEffectComp->Clone()));
    NULL_CHECK(m_pTextureComp = Cast<CTextureComponent*>(rhs.m_pTextureComp->Clone()));
}

HRESULT CPlaneModelComp::Initialize_Prototype(void* pArg)
{
    if (FAILED(__super::Initialize_Prototype(pArg)))
        return E_FAIL;

	return S_OK;
}

HRESULT CPlaneModelComp::Initialize(void* pArg)
{
    TPlaneModelCloneDesc tDesc = {};
    if (nullptr == pArg)
        return E_FAIL;
    tDesc = *ReCast<TPlaneModelCloneDesc*>(pArg);

    m_eMode = tDesc.eMode;

	return S_OK;
}

void CPlaneModelComp::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CPlaneModelComp::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CPlaneModelComp::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CPlaneModelComp::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pEffectComp->Begin(m_vecActivePasses[0]);

    m_pVIBufferComp->Bind_Buffer();

    m_pVIBufferComp->Render_Buffer();

	return S_OK;
}

CPlaneModelComp* CPlaneModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("PlaneModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CPlaneModelComp::Clone(void* pArg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("PlaneModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}


void CPlaneModelComp::Free()
{
    SUPER::Free();

    Safe_Release(m_pVIBufferComp);
    Safe_Release(m_pEffectComp);
    Safe_Release(m_pTextureComp);
}

HRESULT CPlaneModelComp::Bind_ShaderResources()
{
    _float4x4 matTemp = {};
    
    if (m_eMode == ORTHO)
    {
        if (FAILED(Transform().Bind_TransformToEffect(m_pEffectComp, "g_WorldMatrix")))
            return E_FAIL;
        if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Ortho, ECamMatrix::View, ECamNum::One)))))
            return E_FAIL;
        if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Ortho, ECamMatrix::Proj, ECamNum::One)))))
            return E_FAIL;
    }
    else if (m_eMode == PERSP)
    {
        matTemp = Calculate_TransformFloat4x4FromParent();
        if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &matTemp)))
            return E_FAIL;
        if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
            return E_FAIL;
        if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
            return E_FAIL;
        if (FAILED(m_pEffectComp->Bind_RawValue("g_fAlpha", &m_fAlpha, sizeof(_float))))
            return E_FAIL;
    }
    if (FAILED(m_pTextureComp->Bind_SRVToEffect(m_pEffectComp, "g_Texture", m_iCurrentTextureIndex)))
        return E_FAIL;

    return S_OK;
}
