#include "GameObject/UI_Lockon.h"

#include "Component/PlaneModelComp.h"
#include "Component/TextureComponent.h"
#include "Component/EffectComponent.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Character_Common.h"

CUI_Lockon::CUI_Lockon()
{
    Set_Name(TEXT("UI_Lockon"));
    Set_RenderGroup(ERenderGroup::UI);
}

CUI_Lockon::CUI_Lockon(const CUI_Lockon& rhs)
    : Base(rhs)
{
    NULL_CHECK(m_pLockon_Round_Image = DynCast<CPlaneModelComp*>(rhs.m_pLockon_Round_Image->Clone()));
    NULL_CHECK(m_pLockon_Arrow_Image = DynCast<CPlaneModelComp*>(rhs.m_pLockon_Arrow_Image->Clone()));
}

HRESULT CUI_Lockon::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    Transform().Set_Scale(_float3(2.5f, 2.5f, 2.5f));

    return S_OK;
}

HRESULT CUI_Lockon::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    return S_OK;
}

void CUI_Lockon::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CUI_Lockon::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    _vector vCamPos = PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One);
    Transform().Look_At(vCamPos);

    if (nullptr != m_pTarget)
    {
        if (!m_pTarget->IsDead())
        {
            m_fLockon_Lerp += m_fLockon_Speed;
            if (m_fLockon_Lerp > 1.f)
                m_fLockon_Lerp = 1.f;

            _matrix TargetMatrix = m_pTarget->ColliderComp()->Calculate_TransformMatrixFromParent();
            _vector vLerpPos = XMVectorLerp(vCamPos, TargetMatrix.r[3], m_fLockon_Lerp);
            Transform().Set_Position(vLerpPos);
            XMStoreFloat3(&m_vLatest_LockonPos, vLerpPos);
        }
        else
            Set_Dead();
    }
    else
    {
        m_fLockon_Lerp -= m_fLockon_Speed * 1.5f;
        if (m_fLockon_Lerp <= 0)
            Set_Dead();
        else
        {
            _vector vLerpPos = XMVectorLerp(vCamPos, XMLoadFloat3(&m_vLatest_LockonPos), m_fLockon_Lerp);
            Transform().Set_Position(vLerpPos);
        }
    }
        

    m_pLockon_Round_Image->Transform().TurnLook(XMConvertToRadians(360.f) * fTimeDelta);
    m_pLockon_Round_Image->Set_CurrentTextureIndex(0);
    m_pLockon_Round_Image->Set_Alpha(min(m_fLockon_Lerp * 0.75f, 0.75f));
    

    m_pLockon_Arrow_Image->Set_CurrentTextureIndex(0);
    m_pLockon_Arrow_Image->Set_Alpha(min(m_fLockon_Lerp, 1.f));
}

void CUI_Lockon::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_Lockon::Render()
{
    m_pLockon_Round_Image->Render();
    m_pLockon_Arrow_Image->Render();

    return S_OK;
}

CUI_Lockon* CUI_Lockon::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("UI_Lockon Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Lockon::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("UI_Lockon Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CUI_Lockon::Free()
{
    SUPER::Free();

    Safe_Release(m_pTarget);
}

FSerialData CUI_Lockon::SerializeData()
{
    return FSerialData();
}

HRESULT CUI_Lockon::Initialize_Component()
{
    // 프로토타입용 컴포넌트 초기화
    if (FAILED(Add_Component(L"Lockon_Round", m_pLockon_Round_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pLockon_Round_Image->Set_Mode(CPlaneModelComp::PERSP);
    m_pLockon_Round_Image->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Lockon_Round.png"));
    m_pLockon_Round_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pLockon_Round_Image->Reset_ActivePass();
    m_pLockon_Round_Image->Set_ActivePass(1);

    if (FAILED(Add_Component(L"Lockon_Arrow", m_pLockon_Arrow_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pLockon_Arrow_Image->Set_Mode(CPlaneModelComp::PERSP);
    m_pLockon_Arrow_Image->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Lockon_Arrow_%d.png"), 2);
    m_pLockon_Arrow_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pLockon_Arrow_Image->Reset_ActivePass();
    m_pLockon_Arrow_Image->Set_ActivePass(1);

    return S_OK;
}

void CUI_Lockon::Clear_Target()
{
    Safe_Release(m_pTarget);
    m_pTarget = nullptr;
}
