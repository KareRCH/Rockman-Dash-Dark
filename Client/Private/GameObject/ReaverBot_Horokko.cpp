#include "GameObject/ReaverBot_Horokko.h"

#include "Component/EffectComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/Door_Common.h"
#include "Utility/ClassID.h"

CReaverBot_Horokko::CReaverBot_Horokko()
{
	Set_Name(TEXT("ReaverBot_Horokko"));
	m_fHP = FGauge(10.f, true);
	Register_State();
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

CReaverBot_Horokko::CReaverBot_Horokko(const CReaverBot_Horokko& rhs)
	: Base(rhs)
{
	Register_State();
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

HRESULT CReaverBot_Horokko::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.08f, 0.08f, 0.08f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Horokko/Horokko.amodel");
	m_pModelComp->Set_Animation(0, 1.f, true);

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Position(0.f, 0.5f, 0.f);
	m_pColliderComp->Transform().Set_Scale(1.f, 0.5f, 1.f);
	m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
	m_pColliderComp->EnterToPhysics(0);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
		| COLLAYER_ATTACKER | COLLAYER_OBJECT);

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	_uint iNumPrototype = 0;
	iNumPrototype = InputData.Get_ArraySize("Components");
	for (_uint i = 0; i < iNumPrototype; i++)
	{
		FSerialData ProtoData;
		InputData.Get_ObjectFromArray("Components", i, ProtoData);

		_uint iComponentID = 0;
		if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
			return E_FAIL;

		string strName = "";
		if (FAILED(ProtoData.Get_Data("Name", strName)))
			return E_FAIL;

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			NULL_CHECK_RETURN(m_pModelComp = CCommonModelComp::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(ConvertToWstring(strName), m_pModelComp)))
				return E_FAIL;
			m_pModelComp->Set_Animation(0, 1.f, true);
			break;
		case ECast(EComponentID::Collider):
			NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(ConvertToWstring(strName), m_pColliderComp)))
				return E_FAIL;
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			m_pColliderComp->Set_CollisionKinematic();
			//m_pColliderComp->EnterToPhysics(0);
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

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
	if (!m_State_Act.IsOnState(EState_Act::Dead) && m_fHP.Get_Percent() <= 0.f)
		m_State_Act.Set_State(EState_Act::Dead);

	if (!m_fHitTime.Increase(fTimeDelta))
		m_fHitStrength = 0.3f;
	else
		m_fHitStrength = 0.f;

	m_State_AI.Get_StateFunc()(this, fTimeDelta);
	m_State_Act.Get_StateFunc()(this, fTimeDelta);
	m_ActionKey.Reset();

	m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Horokko::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Horokko::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();

		pEffectComp->Bind_RawValue("g_fColorAdd_Strength", VPCast(&m_fHitStrength), sizeof(_float));

		m_pModelComp->Render();
	}

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Reaverbot_Balfura Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("Reaverbot_Balfura Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CReaverBot_Horokko::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Reaverbot_Balfura Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CReaverBot_Horokko::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("Reaverbot_Balfura Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Horokko::Free()
{
	SUPER::Free();
}

FSerialData CReaverBot_Horokko::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("HP", m_fHP.fMax);

	return Data;
}

FSerialData CReaverBot_Horokko::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("HP", m_fHP.fMax);

	return Data;
}

HRESULT CReaverBot_Horokko::Initialize_Component()
{
	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize_Component(FSerialData& InputData)
{
	_uint iNumPrototype = 0;
	iNumPrototype = InputData.Get_ArraySize("Components");
	for (_uint i = 0; i < iNumPrototype; i++)
	{
		FSerialData InputProto;
		InputData.Get_ObjectFromArray("Components", i, InputProto);

		_uint iComponentID = 0;
		if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
			return E_FAIL;

		string strProtoName = "";
		if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
			return E_FAIL;

		string strName = "";
		if (FAILED(InputProto.Get_Data("Name", strName)))
			return E_FAIL;

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
				m_pModelComp = DynCast<CCommonModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
			m_pModelComp->Set_Animation(0, 1.f, true);
			m_pModelComp->Reset_ActivePass();
			m_pModelComp->Set_ActivePass(1);
			break;
		case ECast(EComponentID::Collider):
			FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
				m_pColliderComp = DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			m_pColliderComp->Set_CollisionKinematic();
			m_pColliderComp->EnterToPhysics(0);
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

void CReaverBot_Horokko::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

	CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
	if (pDoor)
	{
		_float3 vNormal(_float(pContact->vContactNormal.x), _float(pContact->vContactNormal.y), _float(pContact->vContactNormal.z));
		_vector vSimNormal = {};
		vSimNormal = XMLoadFloat3(&vNormal);
		Transform().Set_Position((Transform().Get_PositionVector() - vSimNormal * Cast<_float>(pContact->fPenetration)));
		if (XMVectorGetX(XMVector3Dot(-vSimNormal, XMVectorSet(0.f, 1.f, 0.f, 0.f))) < 0.f)
			m_bIsOnGround = true;
	}

	CCharacter_Common* pAttacker = DynCast<CCharacter_Common*>(pDst);
	if (pAttacker)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pAttacker->TeamAgentComp()))
		{
			m_fHitTime.Reset();
		}
	}
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

	if (m_ActionKey.IsAct(EActionKey::LookTarget)
		&& nullptr != m_pTarget)
		Transform().Look_At_OnLand(m_pTarget->Transform().Get_PositionVector(), 5.f * fTimeDelta);
}

void CReaverBot_Horokko::Dead_Effect()
{
	CEffect_Common* pEffect = CEffect_Common::Create();
	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	if (nullptr == pEffect)
		return;

	uniform_real_distribution<_float> RandomPosX(-0.5f, 0.5f);
	uniform_real_distribution<_float> RandomPosY(0.f, 1.f);
	uniform_real_distribution<_float> RandomPosZ(-0.5f, 0.5f);
	pEffect->Transform().Set_Position(Transform().Get_PositionVector()
		+ XMVectorSet(RandomPosX(m_RandomNumber), RandomPosY(m_RandomNumber), RandomPosZ(m_RandomNumber), 0.f));
	GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("boom_small.mp3"), CHANNELID::SOUND_ENEMY_EFFECT, 1.f);
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

	m_State_AI.Add_Func(EState_AI::Idle, &ThisClass::AIState_Idle);
	m_State_AI.Add_Func(EState_AI::Chase, &ThisClass::AIState_Chase);
	m_State_AI.Add_Func(EState_AI::Charge, &ThisClass::AIState_Charge);
	m_State_AI.Add_Func(EState_AI::Charge_Attack, &ThisClass::AIState_Charge_Attack);
	m_State_AI.Add_Func(EState_AI::Prowl, &ThisClass::AIState_Prowl);
	m_State_AI.Add_Func(EState_AI::Dead, &ThisClass::AIState_Dead);
	m_State_AI.Add_Func(EState_AI::Escape, &ThisClass::AIState_Escape);
	m_State_AI.Set_State(EState_AI::Idle);
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
			|| m_ActionKey.IsAct(EActionKey::TurnRight) || m_ActionKey.IsAct(EActionKey::TurnLeft)
			|| m_ActionKey.IsAct(EActionKey::LookTarget))
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
			&& !m_ActionKey.IsAct(EActionKey::TurnRight) && !m_ActionKey.IsAct(EActionKey::TurnLeft)
			&& !m_ActionKey.IsAct(EActionKey::LookTarget))
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
		if (m_fDeadEffect.Increase(fTimeDelta))
		{
			m_fDeadEffect.Reset();
			Dead_Effect();
		}

		if (m_fDeadTime.Increase(fTimeDelta))
			Set_Dead();
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Idle(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(3.f);
	}

	if (m_State_AI.Can_Update())
	{
		// 가만히 있다가 적이 사정권 안에 들어오면 응시하다가 적을 추적한다.
		// 적이 없으면 배회 패턴으로 넘어간다.

		m_pTarget = Find_Target();

		if (nullptr != m_pTarget)
		{
			_vector vLook = Transform().Get_LookNormalizedVector();
			_vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
																	- Transform().Get_PositionVector(), 0.f));
			_float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
			if (fDot < 1.f - 0.001f)
				m_ActionKey.Act(EActionKey::LookTarget);
			else
			{
				if (m_fIdleTime.Increase(fTimeDelta))
				{
					uniform_int_distribution<_uint> RandomPattern(0, 1);
					switch (RandomPattern(m_RandomNumber))
					{
					case 0:
						m_State_AI.Set_State(EState_AI::Chase);
						break;
					case 1:
						m_State_AI.Set_State(EState_AI::Prowl);
						break;
					}
				}
			}
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Chase(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(5.f);
	}

	if (m_State_AI.Can_Update())
	{
		// 적을 직선적으로 추적한다.
		// 일정 거리 안에 들었다면 기를 모은다.
		// 플레이어가 도망갈시 Idle 상태로 변경됨

		m_pTarget = Find_Target();

		if (nullptr != m_pTarget)
		{
			m_ActionKey.Act(EActionKey::LookTarget);
			m_ActionKey.Act(EActionKey::MoveForward);

			if (m_fIdleTime.Increase(fTimeDelta))
			{
				m_State_AI.Set_State(EState_AI::Charge);
			}
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Charge(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(2.5f);
	}

	if (m_State_AI.Can_Update())
	{
		// 사정거리 안에 들어온 적을 바라보며 기를 모은다.
		m_ActionKey.Act(EActionKey::Charge);

		m_pTarget = Find_Target();

		if (nullptr != m_pTarget)
		{
			_vector vLook = Transform().Get_LookNormalizedVector();
			_vector vTargetLook = XMVector3Normalize(XMVectorSetY(m_pTarget->Transform().Get_PositionVector()
				- Transform().Get_PositionVector(), 0.f));
			_float fDot = XMVectorGetX(XMVector3Dot(vLook, vTargetLook));
			if (fDot < 1.f - 0.001f)
			{
				m_ActionKey.Act(EActionKey::LookTarget);
			}
		}

		if (m_fIdleTime.Increase(fTimeDelta))
		{
			m_State_AI.Set_State(EState_AI::Charge_Attack);
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Charge_Attack(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(2.f);
	}

	if (m_State_AI.Can_Update())
	{
		// 적을 바라보며 빙글빙글 돌며 돌진한다.
		m_pTarget = Find_Target();

		if (nullptr != m_pTarget)
		{
			m_ActionKey.Act(EActionKey::LookTarget);
		}

		m_ActionKey.Act(EActionKey::MoveForward);

		if (m_fIdleTime.Increase(fTimeDelta))
		{
			m_ActionKey.Act(EActionKey::Charge);
			m_State_AI.Set_State(EState_AI::Idle);
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Prowl(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(9.f);
		uniform_real_distribution<_float> RandomPatternFloat(2.f, 3.5f);
		m_fTurnTime.Readjust(RandomPatternFloat(m_RandomNumber));
		uniform_int_distribution<_int> RandomPatternInt(0, 1);
		m_fTurnLeft = RandomPatternInt(m_RandomNumber);
	}

	if (m_State_AI.Can_Update())
	{
		// 특정 위치를 기반으로 주변을 배회한다.
		if (m_fTurnTime.Increase(fTimeDelta))
		{
			uniform_int_distribution<_int> RandomPatternInt(0, 1);
			m_fTurnLeft = RandomPatternInt(m_RandomNumber);
			uniform_real_distribution<_float> RandomPatternFloat(0.f, 1.5f);
			m_fTurnTime.Readjust(RandomPatternFloat(m_RandomNumber));
		}

		if (m_fTurnLeft)
		{
			m_ActionKey.Act(EActionKey::TurnLeft);
		}
		else
			m_ActionKey.Act(EActionKey::TurnRight);

		m_ActionKey.Act(EActionKey::MoveForward);

		if (m_fIdleTime.Increase(fTimeDelta))
		{
			uniform_int_distribution<_uint> RandomPattern(0, 1);
			switch (RandomPattern(m_RandomNumber))
			{
			case 0:
				m_State_AI.Set_State(EState_AI::Idle);
				break;
			case 1:
				m_State_AI.Set_State(EState_AI::Chase);
				break;
			}
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Horokko::AIState_Dead(const _float& fTimeDelta)
{
}

void CReaverBot_Horokko::AIState_Escape(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Reset();
	}

	if (m_State_AI.Can_Update())
	{
		// 현재 필드에서 땅속으로 탈출한다.

		if (m_fIdleTime.Increase(fTimeDelta))
		{
			
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

CCharacter_Common* CReaverBot_Horokko::Find_Target()
{
	auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), 10.f, COLLAYER_CHARACTER);
	_float fDistance = FLT_MAX;
	CGameObject* pClosestObj = { nullptr };
	for (auto iter = listObjects.begin(); iter != listObjects.end(); iter++)
	{
		auto pObj = iter->first;
		auto& ContactData = iter->second;
		_float fObjDistance = XMVectorGetX(XMVector3Length((Transform().Get_PositionVector() - pObj->Transform().Get_PositionVector())));
		if (fObjDistance <= fDistance
			&& pObj != this && nullptr != DynCast<CCharacter_Common*>(pObj))
		{
			if (ETeamRelation::Hostile ==
				CTeamAgentComp::Check_Relation(&DynCast<CCharacter_Common*>(pObj)->TeamAgentComp(), &TeamAgentComp()))
			{
				fDistance = fObjDistance;
				pClosestObj = pObj;
			}

		}
	}

	return DynCast<CCharacter_Common*>(pClosestObj);
}
