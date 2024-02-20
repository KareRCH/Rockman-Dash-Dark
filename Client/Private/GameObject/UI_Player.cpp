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


    m_pImage[ENEMY_SPOT]->Set_CurrentTextureIndex(m_fEnemySpot_Index.fCur);
    m_pImage[ENEMY_SPOT]->Set_Alpha(1.f);

    
    m_pImage[HP_FRONT]->Set_MinUV({0.f, 1.f - m_fHP.Get_Percent() });

    m_pImage[SUB_USAGE]->Set_MinUV({ 0.f, 1.f - 1.f });
    m_pImage[SUB_MAIN]->Set_MinUV({ 0.f, 1.f - 1.f });



    if (m_bIsDisplay)
    {
        if (!m_fDisplayLerp.Increase(fTimeDelta * 1.f))
        {
            _vector vOutScreenRight = XMVectorSet(200.f, 0.f, 0.f, 0.f);
            _vector vOutScreenLeft = XMVectorSet(-200.f, 0.f, 0.f, 0.f);
            _vector vTemp = {};

            // 哭率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_FRONT]),
                XMLoadFloat3(&m_vOriginPos[HP_FRONT]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_FRONT]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[ENEMY_SPOT]),
                XMLoadFloat3(&m_vOriginPos[ENEMY_SPOT]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[ENEMY_SPOT]->Transform().Set_Position(vTemp);

            // 坷弗率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[SUB_CONTAINER]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_USAGE]),
                XMLoadFloat3(&m_vOriginPos[SUB_USAGE]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_USAGE]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_MAIN]),
                XMLoadFloat3(&m_vOriginPos[SUB_MAIN]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_MAIN]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[THROW]),
                XMLoadFloat3(&m_vOriginPos[THROW]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[THROW]->Transform().Set_Position(vTemp);
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
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[HP_CONTAINER]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[HP_FRONT]),
                XMLoadFloat3(&m_vOriginPos[HP_FRONT]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[HP_FRONT]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[ENEMY_SPOT]),
                XMLoadFloat3(&m_vOriginPos[ENEMY_SPOT]) + vOutScreenLeft, m_fDisplayLerp.Get_Percent());
            m_pImage[ENEMY_SPOT]->Transform().Set_Position(vTemp);

            // 坷弗率
            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_CONTAINER]),
                XMLoadFloat3(&m_vOriginPos[SUB_CONTAINER]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_CONTAINER]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_USAGE]),
                XMLoadFloat3(&m_vOriginPos[SUB_USAGE]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_USAGE]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[SUB_MAIN]),
                XMLoadFloat3(&m_vOriginPos[SUB_MAIN]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[SUB_MAIN]->Transform().Set_Position(vTemp);

            vTemp = XMVectorLerp(XMLoadFloat3(&m_vOriginPos[THROW]),
                XMLoadFloat3(&m_vOriginPos[THROW]) + vOutScreenRight, m_fDisplayLerp.Get_Percent());
            m_pImage[THROW]->Transform().Set_Position(vTemp);

            
        }
    }
}

void CUI_Player::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_Player::Render()
{
    m_pImage[ENEMY_SPOT]->Render();

    m_pImage[HP_CONTAINER]->Render();
    m_pImage[HP_FRONT]->Render();

    m_pImage[SUB_CONTAINER]->Render();
    m_pImage[SUB_USAGE]->Render();
    m_pImage[SUB_MAIN]->Render();

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

    if (FAILED(Add_Component(L"EnemySpot", m_pImage[ENEMY_SPOT] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[ENEMY_SPOT]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[ENEMY_SPOT]->Transform().Set_Position(-560.f, -280.f, -1.f);
    m_pImage[ENEMY_SPOT]->Transform().Set_Scale(60.f * 1.5f, 72.f * 1.5f, 1.f);
    m_pImage[ENEMY_SPOT]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/EnemySpot/EnemySpot%d.png"), 12);
    m_pImage[ENEMY_SPOT]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[ENEMY_SPOT]->Reset_ActivePass();
    m_pImage[ENEMY_SPOT]->Set_ActivePass(1);
    m_vOriginPos[ENEMY_SPOT] = m_pImage[ENEMY_SPOT]->Transform().Get_PositionFloat3();

    if (FAILED(Add_Component(L"HP_Container", m_pImage[HP_CONTAINER] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[HP_CONTAINER]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[HP_CONTAINER]->Transform().Set_Position(-560.f, -139.f, 1.f);
    m_pImage[HP_CONTAINER]->Transform().Set_Scale(36.f * 1.5f, 116.f * 1.5f, 1.f);
    m_pImage[HP_CONTAINER]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/HP_Container.png"), 1);
    m_pImage[HP_CONTAINER]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[HP_CONTAINER]->Reset_ActivePass();
    m_pImage[HP_CONTAINER]->Set_ActivePass(1);
    m_vOriginPos[HP_CONTAINER] = m_pImage[HP_CONTAINER]->Transform().Get_PositionFloat3();



    if (FAILED(Add_Component(L"HP_Bar", m_pImage[HP_FRONT] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[HP_FRONT]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[HP_FRONT]->Transform().Set_Position(-560.f, -133.f, 0.f);
    m_pImage[HP_FRONT]->Transform().Set_Scale(12.f * 1.5f, 56.f * 1.5f, 1.f);
    m_pImage[HP_FRONT]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/HP_Bar.png"), 1);
    m_pImage[HP_FRONT]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[HP_FRONT]->Reset_ActivePass();
    m_pImage[HP_FRONT]->Set_ActivePass(1);
    m_vOriginPos[HP_FRONT] = m_pImage[HP_FRONT]->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Container", m_pImage[SUB_CONTAINER] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[SUB_CONTAINER]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[SUB_CONTAINER]->Transform().Set_Position(560.f, -160.f, 0.f);
    m_pImage[SUB_CONTAINER]->Transform().Set_Scale(44.f * 1.5f, 231.f * 1.5f, 1.f);
    m_pImage[SUB_CONTAINER]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Container.png"), 1);
    m_pImage[SUB_CONTAINER]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[SUB_CONTAINER]->Reset_ActivePass();
    m_pImage[SUB_CONTAINER]->Set_ActivePass(1);
    m_vOriginPos[SUB_CONTAINER] = m_pImage[SUB_CONTAINER]->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Usage", m_pImage[SUB_USAGE] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[SUB_USAGE]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[SUB_USAGE]->Transform().Set_Position(548.f, -137.f, 0.f);
    m_pImage[SUB_USAGE]->Transform().Set_Scale(8.f * 1.5f, 140.f * 1.5f, 1.f);
    m_pImage[SUB_USAGE]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Usage.png"), 1);
    m_pImage[SUB_USAGE]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[SUB_USAGE]->Reset_ActivePass();
    m_pImage[SUB_USAGE]->Set_ActivePass(1);
    m_vOriginPos[SUB_USAGE] = m_pImage[SUB_USAGE]->Transform().Get_PositionFloat3();


    if (FAILED(Add_Component(L"SubWeapon_Main", m_pImage[SUB_MAIN] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[SUB_MAIN]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[SUB_MAIN]->Transform().Set_Position(570.f, -137.f, 0.f);
    m_pImage[SUB_MAIN]->Transform().Set_Scale(12.f * 1.5f, 140.f * 1.5f, 1.f);
    m_pImage[SUB_MAIN]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/SubWeapon_Main.png"), 1);
    m_pImage[SUB_MAIN]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[SUB_MAIN]->Reset_ActivePass();
    m_pImage[SUB_MAIN]->Set_ActivePass(1);
    m_vOriginPos[SUB_MAIN] = m_pImage[SUB_MAIN]->Transform().Get_PositionFloat3();



    if (FAILED(Add_Component(L"ThrowWeapon", m_pImage[THROW] = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pImage[THROW]->Set_Mode(CPlaneModelComp::ORTHO);
    m_pImage[THROW]->Transform().Set_Position(560.f, -220.f, 0.f);
    m_pImage[THROW]->Transform().Set_Scale(60.f * 1.5f, 112.f * 1.5f, 1.f);
    m_pImage[THROW]->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Gauge/ThrowWeapon%d.png"), 2);
    m_pImage[THROW]->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pImage[THROW]->Reset_ActivePass();
    m_pImage[THROW]->Set_ActivePass(1);
    m_vOriginPos[THROW] = m_pImage[THROW]->Transform().Get_PositionFloat3();


    return S_OK;
}

void CUI_Player::Update_CloudStation()
{
    if (nullptr != m_pPlayerCloud)
    {
        m_fHP = m_pPlayerCloud->Get_HP();
    }
}
