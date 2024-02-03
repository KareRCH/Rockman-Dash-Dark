#include "GameObject/UI_Boss.h"

#include "Component/CloudStationComp.h"
#include "Component/PlaneModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

#include "CloudStation/CloudStation_Boss.h"

CUI_Boss::CUI_Boss()
{
    Set_Name(TEXT("UI_Boss"));
}

CUI_Boss::CUI_Boss(const CUI_Boss& rhs)
{
}

HRESULT CUI_Boss::Initialize_Prototype()
{
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Boss::Initialize(void* Arg)
{
    return S_OK;
}

void CUI_Boss::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CUI_Boss::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    Update_CloudStation();

    m_pImage[HP_BAR]->Set_MinUV({ 1.f - m_fHP.Get_Percent(), 0.f});
    m_bIsDisplay = !(m_fHP.Get_Percent() > 0.f);

    if (m_fFontDisplayTime.Update(fTimeDelta))
    {

    }

    if (m_bIsDisplay)
    {
        if (!m_fDisplayLerp.Increase(fTimeDelta * 1.f))
        {
            _vector vOutScreenRight = XMVectorSet(500.f, 0.f, 0.f, 0.f);
            _vector vTemp = {};

            // 왼쪽
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_BAR]),
                XMLoadFloat3(&m_vOriginPos[HP_BAR]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_BAR]->Transform().Set_Position(vTemp);
        }

        if (m_fDisplayLerp.IsMax())
            Set_Dead();
    }
    else
    {
        if (!m_fDisplayLerp.Decrease(fTimeDelta * 1.f))
        {
            _vector vOutScreenRight = XMVectorSet(500.f, 0.f, 0.f, 0.f);
            _vector vTemp = {};

            // 왼쪽
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_BAR]),
                XMLoadFloat3(&m_vOriginPos[HP_BAR]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_BAR]->Transform().Set_Position(vTemp);
        }
    }
}

void CUI_Boss::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_Boss::Render()
{
    m_pImage[HP_CONTAINER]->Render();
    m_pImage[HP_BAR]->Render();

    if (!m_fFontDisplayTime.IsFinished())
    {
        m_pGI->Render_Font(L"Boss", L"유적의 문지기", _float2(465.f, 100.f), XMVectorSet(1.f, 1.f, 1.f, m_fFontDisplayTime.ClampPercent(1.5f)), 1.2f * 0.62f);
        m_pGI->Render_Font(L"Boss", L"한길 인형", _float2(440.f, 180.f), XMVectorSet(1.f, 1.f, 1.f, m_fFontDisplayTime.ClampPercent(1.5f)), 2.f * 0.62f);
    }

    return S_OK;
}

void CUI_Boss::OnCreated()
{
    SUPER::OnCreated();

    Set_RenderGroup(ERenderGroup::UI);
}

void CUI_Boss::BeginPlay()
{
    SUPER::BeginPlay();

    if (nullptr == m_pBossCloud)
    {
        if (SUCCEEDED(m_pCloudStationComp->Connect_CloudStation(TEXT("Boss"))))
        {
            m_pBossCloud = m_pCloudStationComp->Get_LastCloudStation<CCloudStation_Boss>();
            Safe_AddRef(m_pBossCloud);
        }
    }

    m_fDisplayLerp.Set_Max();
}

CUI_Boss* CUI_Boss::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Boss Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Boss::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Boss Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CUI_Boss::Free()
{
    SUPER::Free();

    Safe_Release(m_pBossCloud);
}

FSerialData CUI_Boss::SerializeData()
{
    return FSerialData();
}

HRESULT CUI_Boss::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"CloudStation", m_pCloudStationComp = CCloudStationComp::Create()), E_FAIL);

    if (FAILED(Add_Component(L"HP_Container", m_pImage[HP_CONTAINER] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[HP_CONTAINER]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[HP_CONTAINER]->Transform().Set_Position(360.f, 300.f, 1.f);
    m_pImage[HP_CONTAINER]->Transform().Set_Scale(512.f * 1.f, 64.f * 1.f, 1.f);
    m_pImage[HP_CONTAINER]->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/BossHP/BossHP_Container.png"), 1);
    m_pImage[HP_CONTAINER]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[HP_CONTAINER]->Reset_ActivePass();
    m_pImage[HP_CONTAINER]->Set_ActivePass(1);
    m_vOriginPos[HP_CONTAINER] = m_pImage[HP_CONTAINER]->Transform().Get_PositionFloat3();



    if (FAILED(Add_Component(L"HP_Bar", m_pImage[HP_BAR] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[HP_BAR]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[HP_BAR]->Transform().Set_Position(392.f, 302.f, 1.f);
    m_pImage[HP_BAR]->Transform().Set_Scale(320.f * 1.f, 20.f * 1.f, 1.f);
    m_pImage[HP_BAR]->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/BossHP/BossHP_Bar.png"), 1);
    m_pImage[HP_BAR]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[HP_BAR]->Reset_ActivePass();
    m_pImage[HP_BAR]->Set_ActivePass(1);
    m_vOriginPos[HP_BAR] = m_pImage[HP_BAR]->Transform().Get_PositionFloat3();

    return S_OK;
}

void CUI_Boss::Update_CloudStation()
{
    if (nullptr != m_pBossCloud)
    {
        m_pBossCloud->Access_HP(CCloudStation::EMode::Download, m_fHP);
    }
}
