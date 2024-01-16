#include "Component/BoxModelComp.h"

#include "Component/BoxBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"


CBoxModelComp::CBoxModelComp()
{
    // InternalComponent는 생성시 자동 포함 하도록 설계된다.
    // 이들은 반드시 생성되어야 한다는 규칙을 가진다.
    // 이들에 대한 접근은 reference로 이루어진다.
    NULL_CHECK(m_pVIBufferComp = CBoxBufferComp::Create());
    NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
    NULL_CHECK(m_pTextureComp = CTextureComponent::Create());
}

CBoxModelComp::CBoxModelComp(const CBoxModelComp& rhs)
    : Base(rhs)
{
    // InternalComponent 중 얕은 복사, 혹은 깊은 복사가 필요한 경우 여기서 처리하도록 한다.
    // 얕은 복사는 주로 Device, Pipeline과 같은 시스템 접근 컴포넌트가 해당된다.
    // 나머지는 깊은 복사로 진행된다. (버퍼, 셰이더, 텍스처와 같이 공유되는 리소스를 가진 컴포넌트들은
    // 내부적으로 얕은 복사가 일어난다.
    NULL_CHECK(m_pVIBufferComp = Cast<CBoxBufferComp*>(rhs.m_pVIBufferComp->Clone()));
    NULL_CHECK(m_pEffectComp = Cast<CEffectComponent*>(rhs.m_pEffectComp->Clone()));
    NULL_CHECK(m_pTextureComp = Cast<CTextureComponent*>(rhs.m_pTextureComp->Clone()));
}

HRESULT CBoxModelComp::Initialize_Prototype(void* Arg)
{
    if (FAILED(__super::Initialize_Prototype(Arg)))
        return E_FAIL;

	return S_OK;
}

HRESULT CBoxModelComp::Initialize_Prototype(FSerialData& InputData)
{
    if (FAILED(__super::Initialize_Prototype(InputData)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoxModelComp::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CBoxModelComp::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

    return S_OK;
}

void CBoxModelComp::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CBoxModelComp::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CBoxModelComp::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);
    
    
}

HRESULT CBoxModelComp::Render()
{
    Bind_ShaderResources();

    for (_uint i = 0; i < m_iNumActivePasses; i++)
    {
        m_pEffectComp->Begin(m_vecActivePasses[i]);

        m_pVIBufferComp->Bind_Buffer();

        m_pVIBufferComp->Render_Buffer();
    }

	return S_OK;
}

FSerialData CBoxModelComp::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

FSerialData CBoxModelComp::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ComponentID", g_ClassID);

    return Data;
}

CBoxModelComp* CBoxModelComp::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CBoxModelComp* CBoxModelComp::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CBoxModelComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

CComponent* CBoxModelComp::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CBoxModelComp::Free()
{
    SUPER::Free();

    Safe_Release(m_pVIBufferComp);
    Safe_Release(m_pEffectComp);
    Safe_Release(m_pTextureComp);
}

HRESULT CBoxModelComp::Bind_ShaderResources()
{
    _float4x4 matTemp = {};

    _float4x4 matParent = Calculate_TransformFloat4x4FromParent();
    XMStoreFloat4x4(&matTemp, Transform().Get_TransformMatrix() * XMLoadFloat4x4(&matParent));

    if (FAILED(m_pEffectComp->Bind_Matrix("g_WorldMatrix", &matTemp)))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
        return E_FAIL;
    if (FAILED(m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
        return E_FAIL;
    if (FAILED(m_pTextureComp->Bind_SRVToEffect(m_pEffectComp, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}
