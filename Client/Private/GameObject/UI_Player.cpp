#include "GameObject/UI_Player.h"

#include "Component/CloudStationComp.h"
#include "Component/PlaneModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CUI_Player::CUI_Player()
{
    Set_Name(TEXT("UI_Player"));
    Set_RenderGroup(ERenderGroup::UI);
}

CUI_Player::CUI_Player(const CUI_Player& rhs)
{
}

HRESULT CUI_Player::Initialize_Prototype()
{
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player::Initialize(void* Arg)
{
    return S_OK;
}

void CUI_Player::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

    //m_pCloudStationComp->Priority_Tick(fTimeDelta);
    if (nullptr == m_pPlayerCloud)
    {
        if (SUCCEEDED(m_pCloudStationComp->Connect_CloudStation(TEXT("Player"))))
        {
            m_pPlayerCloud = m_pCloudStationComp->Get_LastCloudStation<CCloudStation_Player>();
            Safe_AddRef(m_pPlayerCloud);
        }
    }
}

void CUI_Player::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    Update_CloudStation();

    if (GI()->IsKey_Pressed(DIK_RETURN))
        m_bIsDisplay = !m_bIsDisplay;

    if (!m_bIsEnemySpoted)
    {
        if (m_fEnemySpot_Index.Increase_UpToPoint(fTimeDelta * 10.f, 6.f))
            m_fEnemySpot_Index.Reset();
    }
    else
    {
        if (m_fEnemySpot_Index.Increase(fTimeDelta * 50.f))
            m_fEnemySpot_Index.Reset(6.f);
    }


    m_pEnemySpot_Image->Set_CurrentTextureIndex(m_fEnemySpot_Index.fCur);
    m_pEnemySpot_Image->Set_Alpha(1.f);

    
    m_pHP_Frontbar_Image->Set_MinUV({0.f, 1.f - m_fHP.Get_Percent() });

    m_pSubWeapon_Usage_Image->Set_MinUV({ 0.f, 1.f - m_fHP.Get_Percent() });
    m_pSubWeapon_Main_Image->Set_MinUV({ 0.f, 1.f - m_fHP.Get_Percent() });



    if (m_bIsDisplay)
    {
        if (!m_fDisplayLerp.Increase(fTimeDelta * 1.f))
        {
            _vector vOutScreenRight = XMVectorSet(200.f, 0.f, 0.f, 0.f);
            _vector vOutScreenLeft = XMVectorSet(-200.f, 0.f, 0.f, 0.f);
            _vector vTemp = {};

            // 哭率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vHP_Container_OriginPos),
                XMLoadFloat3(&m_vHP_Container_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pHP_Container_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vHP_Frontbar_OriginPos),
                XMLoadFloat3(&m_vHP_Frontbar_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pHP_Frontbar_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vEnemySpot_OriginPos),
                XMLoadFloat3(&m_vEnemySpot_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pEnemySpot_Image->Transform().Set_Position(vTemp);

            // 坷弗率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Container_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Container_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Container_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Usage_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Usage_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Usage_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Main_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Main_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Main_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vThrowWeapon_OriginPos),
                XMLoadFloat3(&m_vThrowWeapon_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pThrowWeapon_Image->Transform().Set_Position(vTemp);
        }
    }
    else
    {
        if (!m_fDisplayLerp.Decrease(fTimeDelta * 1.f))
        {
            _vector vOutScreenRight = XMVectorSet(200.f, 0.f, 0.f, 0.f);
            _vector vOutScreenLeft = XMVectorSet(-200.f, 0.f, 0.f, 0.f);
            _vector vTemp = {};

            // 哭率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vHP_Container_OriginPos),
                XMLoadFloat3(&m_vHP_Container_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pHP_Container_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vHP_Frontbar_OriginPos),
                XMLoadFloat3(&m_vHP_Frontbar_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pHP_Frontbar_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vEnemySpot_OriginPos),
                XMLoadFloat3(&m_vEnemySpot_OriginPos) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pEnemySpot_Image->Transform().Set_Position(vTemp);

            // 坷弗率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Container_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Container_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Container_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Usage_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Usage_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Usage_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vSubWeapon_Main_OriginPos),
                XMLoadFloat3(&m_vSubWeapon_Main_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pSubWeapon_Main_Image->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vThrowWeapon_OriginPos),
                XMLoadFloat3(&m_vThrowWeapon_OriginPos) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pThrowWeapon_Image->Transform().Set_Position(vTemp);

            
        }
    }
}

void CUI_Player::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    
}

HRESULT CUI_Player::Render()
{
    m_pEnemySpot_Image->Render();

    m_pHP_Container_Image->Render();
    m_pHP_Frontbar_Image->Render();

    m_pSubWeapon_Container_Image->Render();
    m_pSubWeapon_Usage_Image->Render();
    m_pSubWeapon_Main_Image->Render();

    //m_pThrowWeapon_Image->Render();
    
    return S_OK;
}

CUI_Player* CUI_Player::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CUI_Player::Free()
{
    SUPER::Free();

    Safe_Release(m_pPlayerCloud);
}

FSerialData CUI_Player::SerializeData()
{
    return FSerialData();
}

HRESULT CUI_Player::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"CloudStation", m_pCloudStationComp = CCloudStationComp::Create()), E_FAIL);

    if (FAILED(Add_Component(L"EnemySpot", m_pEnemySpot_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pEnemySpot_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pEnemySpot_Image->Transform().Set_Position(-560.f, -280.f, -1.f);
    m_pEnemySpot_Image->Transform().Set_Scale(60.f * 1.5f, 72.f * 1.5f, 1.f);
    m_pEnemySpot_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/EnemySpot/EnemySpot%d.png"), 12);
    m_pEnemySpot_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pEnemySpot_Image->Reset_ActivePass();
    m_pEnemySpot_Image->Set_ActivePass(1);
    m_vEnemySpot_OriginPos = m_pEnemySpot_Image->Transform().Get_PositionFloat3();

    if (FAILED(Add_Component(L"HP_Container", m_pHP_Container_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pHP_Container_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pHP_Container_Image->Transform().Set_Position(-560.f, -139.f, 1.f);
    m_pHP_Container_Image->Transform().Set_Scale(36.f * 1.5f, 116.f * 1.5f, 1.f);
    m_pHP_Container_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/HP_Container.png"), 1);
    m_pHP_Container_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pHP_Container_Image->Reset_ActivePass();
    m_pHP_Container_Image->Set_ActivePass(1);
    m_vHP_Container_OriginPos = m_pHP_Container_Image->Transform().Get_PositionFloat3();



    if (FAILED(Add_Component(L"HP_Bar", m_pHP_Frontbar_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pHP_Frontbar_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pHP_Frontbar_Image->Transform().Set_Position(-560.f, -133.f, 0.f);
    m_pHP_Frontbar_Image->Transform().Set_Scale(12.f * 1.5f, 56.f * 1.5f, 1.f);
    m_pHP_Frontbar_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/HP_Bar.png"), 1);
    m_pHP_Frontbar_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pHP_Frontbar_Image->Reset_ActivePass();
    m_pHP_Frontbar_Image->Set_ActivePass(1);
    m_vHP_Frontbar_OriginPos = m_pHP_Frontbar_Image->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Container", m_pSubWeapon_Container_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pSubWeapon_Container_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pSubWeapon_Container_Image->Transform().Set_Position(560.f, -160.f, 0.f);
    m_pSubWeapon_Container_Image->Transform().Set_Scale(44.f * 1.5f, 231.f * 1.5f, 1.f);
    m_pSubWeapon_Container_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Container.png"), 1);
    m_pSubWeapon_Container_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pSubWeapon_Container_Image->Reset_ActivePass();
    m_pSubWeapon_Container_Image->Set_ActivePass(1);
    m_vSubWeapon_Container_OriginPos = m_pSubWeapon_Container_Image->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Usage", m_pSubWeapon_Usage_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pSubWeapon_Usage_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pSubWeapon_Usage_Image->Transform().Set_Position(548.f, -137.f, 0.f);
    m_pSubWeapon_Usage_Image->Transform().Set_Scale(8.f * 1.5f, 140.f * 1.5f, 1.f);
    m_pSubWeapon_Usage_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Usage.png"), 1);
    m_pSubWeapon_Usage_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pSubWeapon_Usage_Image->Reset_ActivePass();
    m_pSubWeapon_Usage_Image->Set_ActivePass(1);
    m_vSubWeapon_Usage_OriginPos = m_pSubWeapon_Usage_Image->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Main", m_pSubWeapon_Main_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pSubWeapon_Main_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pSubWeapon_Main_Image->Transform().Set_Position(570.f, -137.f, 0.f);
    m_pSubWeapon_Main_Image->Transform().Set_Scale(12.f * 1.5f, 140.f * 1.5f, 1.f);
    m_pSubWeapon_Main_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Main.png"), 1);
    m_pSubWeapon_Main_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pSubWeapon_Main_Image->Reset_ActivePass();
    m_pSubWeapon_Main_Image->Set_ActivePass(1);
    m_vSubWeapon_Main_OriginPos = m_pSubWeapon_Main_Image->Transform().Get_PositionFloat3();



    if (FAILED(Add_Component(L"ThrowWeapon", m_pThrowWeapon_Image = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pThrowWeapon_Image->Set_Mode(CPlaneModelComp::ORTHO);
    m_pThrowWeapon_Image->Transform().Set_Position(560.f, -220.f, 0.f);
    m_pThrowWeapon_Image->Transform().Set_Scale(60.f * 1.5f, 112.f * 1.5f, 1.f);
    m_pThrowWeapon_Image->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/ThrowWeapon%d.png"), 2);
    m_pThrowWeapon_Image->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pThrowWeapon_Image->Reset_ActivePass();
    m_pThrowWeapon_Image->Set_ActivePass(1);
    m_vThrowWeapon_OriginPos = m_pThrowWeapon_Image->Transform().Get_PositionFloat3();


    return S_OK;
}

void CUI_Player::Update_CloudStation()
{
    if (nullptr != m_pPlayerCloud)
    {
        m_fHP = m_pPlayerCloud->Get_HP();
    }
}
