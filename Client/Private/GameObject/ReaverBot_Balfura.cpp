#include "GameObject/ReaverBot_Balfura.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CReaverBot_Balfura::CReaverBot_Balfura()
{
    Set_Name(TEXT("ReaverBot_Balfura"));
    Set_RenderGroup(ERenderGroup::NonBlend);
}

CReaverBot_Balfura::CReaverBot_Balfura(const CReaverBot_Balfura& rhs)
{
}

HRESULT CReaverBot_Balfura::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize_Prototype(const _float3 vPos)
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    Transform().Set_Position(vPos);

    //Register_State();

    m_pModelComp->Set_Animation(0, 1.f, true);

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    return S_OK;
}

HRESULT CReaverBot_Balfura::Initialize(const _float3 vPos)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    Transform().Set_Position(vPos);

    return S_OK;
}

void CReaverBot_Balfura::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Balfura::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

    //Input_ActionKey();
    ////m_State_AI.Get_StateFunc()(this, fTimeDelta);
    //m_State_Act.Get_StateFunc()(this, fTimeDelta);
    //m_ActionKey.Reset();

    m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Balfura::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    m_pModelComp->Add_AnimTime(fTimeDelta);
    m_pModelComp->Invalidate_Animation();
    m_pModelComp->Invalidate_BoneTransforms();

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Balfura::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

#ifdef _DEBUG
    GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

    return S_OK;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CReaverBot_Balfura* CReaverBot_Balfura::Create(const _float3 vPos)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(vPos)))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CReaverBot_Balfura::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Balfura::Free()
{
    SUPER::Free();
}

FSerialData CReaverBot_Balfura::SerializeData()
{
    return FSerialData();
}

HRESULT CReaverBot_Balfura::Initialize_Component()
{
    FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

    m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(180.f));
    m_pModelComp->Transform().Set_Scale(_float3(0.32f, 0.32f, 0.32f));
    m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
    m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Balfura/Balfura.amodel");

    if (nullptr == m_pColliderComp)
        return E_FAIL;
    m_pColliderComp->Bind_Collision(ECollisionType::OBB);
    m_pColliderComp->EnterToPhysics(0);
    m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
    m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
                                        | COLLAYER_ATTACKER | COLLAYER_OBJECT);

    TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

    return S_OK;
}

void CReaverBot_Balfura::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

}

void CReaverBot_Balfura::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CReaverBot_Balfura::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}
