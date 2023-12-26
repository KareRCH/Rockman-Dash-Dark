#include "GameObject/Player.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"

#include "System/RenderMgr.h"

CPlayer::CPlayer()
{
    Set_Name(L"Player");
}

CPlayer::CPlayer(const CPlayer& rhs)
    : Base(rhs)
    , m_pModelComp(rhs.m_pModelComp)
{
}

HRESULT CPlayer::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용

    m_pModelComp->Set_Animation(0, true);

    return S_OK;
}

HRESULT CPlayer::Initialize_Prototype(const _float3 vPos)
{
    FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용

    Transform().Set_Position(vPos);

    m_pModelComp->Set_Animation(0, true);

    return S_OK;
}

HRESULT CPlayer::Initialize(void* Arg)
{
    FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    m_pModelComp->Set_Animation(0, true);

    return S_OK;
}

HRESULT CPlayer::Initialize(const _float3 vPos)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    Transform().Set_Position(vPos);
    //Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    m_pModelComp->Set_Animation(0, true);

    return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_UP))
        Transform().MoveForward(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_DOWN))
        Transform().MoveForward(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressing(DIK_RIGHT))
        Transform().TurnRight(5.f * fTimeDelta);
    else if (GI()->IsKey_Pressing(DIK_LEFT))
        Transform().TurnRight(-5.f * fTimeDelta);

    if (GI()->IsKey_Pressed(DIK_RETURN))
        Toggle_State(EGObjectState::Render);
    if (GI()->IsKey_Pressed(DIK_BACK))
        Set_Dead();
    
    Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
    
    _float3 t = Transform().Get_RotationEulerFloat3();

    if (m_Gauge.Increase(fTimeDelta))
    {
        
        m_Gauge.Reset();
    }

    if (GI()->IsKey_Pressed(DIK_T))
    {
        GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("rockman_jump.mp3"), CHANNELID::SOUND_EFFECT, 1.f);
        m_bTest = !m_bTest;
        if (!m_bTest)
            m_pModelComp->Set_Animation(1, true);
        else
            m_pModelComp->Set_Animation(2, true);
    }
    
    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();
    m_pModelComp->Invalidate_BoneTransforms();
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CPlayer::Render()
{
    SUPER::Render();
    
    m_pModelComp->Render();

    return S_OK;
}

CPlayer* CPlayer::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CPlayer* CPlayer::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("TestObject Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CPlayer::Free()
{
    SUPER::Free();
}

HRESULT CPlayer::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
    //m_TriBufferComp->Set_StateRender(ECOMP_UPDATE_T::SEMI_AUTO);

    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Megaman/Megaman.amodel");

    /*m_pModelComp->Create_Mask(L"Main", L"Armature", true);
    m_pModelComp->Create_Mask(L"Head", L"Armature", false);
    m_pModelComp->Create_Mask(L"Leg", L"Armature", false);
    m_pModelComp->Create_Mask(L"LeftArm", L"Armature", false);
    m_pModelComp->Create_Mask(L"RightArm", L"Armature", false);

    m_pModelComp->Active_BoneMask(2, L"bone_000");*/


    return S_OK;
}
