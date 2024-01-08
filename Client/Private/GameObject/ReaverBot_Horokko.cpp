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
	Set_RenderGroup(ERenderGroup::Alpha);
}

CReaverBot_Horokko::CReaverBot_Horokko(const CReaverBot_Horokko& rhs)
	: Base(rhs)
{
	NULL_CHECK(m_pModelComp = DynCast<CCommonModelComp*>(rhs.m_pModelComp->Clone()));
}

HRESULT CReaverBot_Horokko::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize_Prototype(const _float3 vPos)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);
	
	Register_State();

	m_pModelComp->Set_Animation(0, 1.f, true);

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(const _float3 vPos)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

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

	Input_ActionKey();
	//m_State_AI.Get_StateFunc()(this, fTimeDelta);
	m_State_Act.Get_StateFunc()(this, fTimeDelta);
	m_ActionKey.Reset();

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

#ifdef _DEBUG
	m_pColliderComp->Render();
#endif

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

	m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.08f, 0.08f, 0.08f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Horokko/Horokko.amodel");

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

void CReaverBot_Horokko::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);
}

void CReaverBot_Horokko::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);
}

void CReaverBot_Horokko::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);
}

void CReaverBot_Horokko::Input_ActionKey()
{
	if (!m_bCanControl)
		return;

	if (GI()->IsKey_Pressing(DIK_UPARROW))
		m_ActionKey.Act(EActionKey::MoveForward);
	if (GI()->IsKey_Pressing(DIK_DOWNARROW))
		m_ActionKey.Act(EActionKey::MoveBackward);
	if (GI()->IsKey_Pressing(DIK_RIGHTARROW))
		m_ActionKey.Act(EActionKey::TurnRight);
	if (GI()->IsKey_Pressing(DIK_LEFTARROW))
		m_ActionKey.Act(EActionKey::TurnLeft);

	if (!m_State_Act.IsOnState(EState_Act::Ready_Shooting)
		&& !m_State_Act.IsOnState(EState_Act::Shooting)
		&& !m_State_Act.IsOnState(EState_Act::End_Shooting))
	{
		if (m_State_Act.IsOnState(EState_Act::Ready_Charge))
		{
			if (GI()->IsKey_Pressing(DIK_J))
				m_ActionKey.Act(EActionKey::Charge);
		}
		else
		{
			if (GI()->IsKey_Pressed(DIK_J))
				m_ActionKey.Act(EActionKey::Charge);
		}
	}
	else
	{
		if (GI()->IsKey_Pressed(DIK_J))
			m_ActionKey.Act(EActionKey::ShootBomb);
	}
	if (GI()->IsKey_Pressed(DIK_K))
		m_ActionKey.Act(EActionKey::ReadyBomb);
}

void CReaverBot_Horokko::Move_Update(const _float& fTimeDelta)
{
	if (m_ActionKey.IsAct(EActionKey::MoveForward))
		Transform().MoveForward(m_fMoveSpeed * fTimeDelta);
	else if (m_ActionKey.IsAct(EActionKey::MoveBackward))
		Transform().MoveForward(-m_fMoveSpeed * fTimeDelta);

	if (m_ActionKey.IsAct(EActionKey::TurnRight))
		Transform().TurnRight(m_fMoveSpeed * fTimeDelta);
	else if (m_ActionKey.IsAct(EActionKey::TurnLeft))
		Transform().TurnRight(-m_fMoveSpeed * fTimeDelta);
}

void CReaverBot_Horokko::Register_State()
{
	for (_uint i = 0; i < ECast(EActionKey::Size); i++)
		m_ActionKey.Add_Action(Cast<EActionKey>(i));

	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::Run, &ThisClass::ActState_Run);
	m_State_Act.Add_Func(EState_Act::Ready_Charge, &ThisClass::ActState_Ready_Charge);
	m_State_Act.Add_Func(EState_Act::Charge_Attack, &ThisClass::ActState_Charge_Attack);
	m_State_Act.Add_Func(EState_Act::Ready_Shooting, &ThisClass::ActState_Ready_Shooting);
	m_State_Act.Add_Func(EState_Act::Shooting, &ThisClass::ActState_Shooting);
	m_State_Act.Add_Func(EState_Act::End_Shooting, &ThisClass::ActState_End_Shooting);
	m_State_Act.Add_Func(EState_Act::Dead, &ThisClass::ActState_Dead);
	m_State_Act.Set_State(EState_Act::Idle);
}

void CReaverBot_Horokko::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_fMoveSpeed = 2.5f;
		m_pModelComp->Set_Animation(2, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{

		// 돌진
		if (m_ActionKey.IsAct(EActionKey::Charge))
			m_State_Act.Set_State(EState_Act::Ready_Charge);
		// 슈팅
		if (m_ActionKey.IsAct(EActionKey::ReadyBomb))
			m_State_Act.Set_State(EState_Act::Ready_Shooting);
		// 움직임
		if (m_ActionKey.IsAct(EActionKey::MoveForward) || m_ActionKey.IsAct(EActionKey::MoveBackward)
			|| m_ActionKey.IsAct(EActionKey::TurnRight) || m_ActionKey.IsAct(EActionKey::TurnLeft))
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
		m_pModelComp->Set_Animation(7, 1.5f, true);
	}

	if (m_State_Act.Can_Update())
	{
		Move_Update(fTimeDelta);

		// 돌진
		if (m_ActionKey.IsAct(EActionKey::Charge))
			m_State_Act.Set_State(EState_Act::Ready_Charge);
		// 슈팅
		if (m_ActionKey.IsAct(EActionKey::ReadyBomb))
			m_State_Act.Set_State(EState_Act::Ready_Shooting);
		// 움직임
		if (!m_ActionKey.IsAct(EActionKey::MoveForward) && !m_ActionKey.IsAct(EActionKey::MoveBackward)
			&& !m_ActionKey.IsAct(EActionKey::TurnRight) && !m_ActionKey.IsAct(EActionKey::TurnLeft))
			m_State_Act.Set_State(EState_Act::Idle);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}


void CReaverBot_Horokko::ActState_Ready_Charge(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_fMoveSpeed = 5.f;
		m_pModelComp->Set_Animation(3, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{

		// 떼면 돌진
		if (!m_ActionKey.IsAct(EActionKey::Charge))
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
		Move_Update(fTimeDelta);

		// 떼면 돌진, 다시 Charge를 누르면 Idle로
		if (m_ActionKey.IsAct(EActionKey::Charge))
			m_State_Act.Set_State(EState_Act::Idle);
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

		if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
		{
			if (m_ActionKey.IsAct(EActionKey::ReadyBomb))
				m_State_Act.Set_State(EState_Act::End_Shooting);
			if (m_ActionKey.IsAct(EActionKey::ShootBomb))
				m_State_Act.Set_State(EState_Act::Shooting);
		}
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

		if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
		{
			if (m_ActionKey.IsAct(EActionKey::ReadyBomb))
				m_State_Act.Set_State(EState_Act::End_Shooting);
			if (m_ActionKey.IsAct(EActionKey::ShootBomb))
				m_State_Act.Set_State(EState_Act::Shooting);
		}
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
		if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
			m_State_Act.Set_State(EState_Act::Idle);
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
		if (m_pModelComp->AnimationComp()->IsAnimation_Finished())
			Set_Dead();
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}
