#include "GameObject/ReaverBot_Horokko.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

CReaverBot_Horokko::CReaverBot_Horokko()
{
	Set_Name(TEXT("ReaverBot_Horokko"));
}

CReaverBot_Horokko::CReaverBot_Horokko(const CReaverBot_Horokko& rhs)
{
}

HRESULT CReaverBot_Horokko::Initialize_Prototype()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize_Prototype(const _float3 vPos)
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	Transform().Set_Position(vPos);

	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::Run, &ThisClass::ActState_Run);
	m_State_Act.Add_Func(EState_Act::Ready_Charge, &ThisClass::ActState_Ready_Charge);
	m_State_Act.Add_Func(EState_Act::Charge_Attack, &ThisClass::ActState_Charge_Attack);
	m_State_Act.Add_Func(EState_Act::Ready_Shooting, &ThisClass::ActState_Ready_Shooting);
	m_State_Act.Add_Func(EState_Act::Shooting, &ThisClass::ActState_Shooting);
	m_State_Act.Add_Func(EState_Act::End_Shooting, &ThisClass::ActState_End_Shooting);
	m_State_Act.Add_Func(EState_Act::Dead, &ThisClass::ActState_Dead);

	m_pModelComp->Set_Animation(0, 1.f, true);

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(const _float3 vPos)
{
	Transform().Set_Position(vPos);

	return S_OK;
}

void CReaverBot_Horokko::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Horokko::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	//m_State_AI.Get_StateFunc()(this, fTimeDelta);
	m_State_Act.Get_StateFunc()(this, fTimeDelta);

	m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Horokko::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();
	m_pModelComp->Invalidate_BoneTransforms();

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Horokko::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

	return S_OK;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CReaverBot_Horokko::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Horokko::Free()
{
	SUPER::Free();
}

HRESULT CReaverBot_Horokko::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

	m_pModelComp->Transform().Set_Scale(_float3(0.08f, 0.08f, 0.08f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Horokko/Horokko.amodel");

	if (nullptr == m_pColliderComp)
		return E_FAIL;
	m_pColliderComp->Bind_Collision(ECollisionType::Box);
	m_pColliderComp->EnterToPhysics(0);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
										| COLLAYER_OBJECT);

	return S_OK;
}

void CReaverBot_Horokko::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	cout << "몬스터 충돌함" << endl;
}

void CReaverBot_Horokko::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	cout << "몬스터 충돌진입" << endl;
}

void CReaverBot_Horokko::OnCollisionExited(CGameObject* pDst)
{
	cout << "몬스터 충돌나감" << endl;
}

void CReaverBot_Horokko::Move_Update(const _float& fTimeDelta)
{

}

void CReaverBot_Horokko::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(2, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Run);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::ActState_Run(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(7, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Ready_Charge);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}


void CReaverBot_Horokko::ActState_Ready_Charge(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(3, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Charge_Attack);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::ActState_Charge_Attack(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(0, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Ready_Shooting);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}


void CReaverBot_Horokko::ActState_Ready_Shooting(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(6, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Shooting);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::ActState_Shooting(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(5, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::End_Shooting);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::ActState_End_Shooting(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(4, 1.f, false, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Dead);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::ActState_Dead(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(1, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{
		if (GI()->IsKey_Pressed(DIK_RETURN))
			m_State_Act.Set_State(EState_Act::Idle);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}
