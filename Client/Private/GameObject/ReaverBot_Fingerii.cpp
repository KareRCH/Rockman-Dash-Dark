#include "GameObject/ReaverBot_Fingerii.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "Utility/ClassID.h"


CReaverBot_Fingerii::CReaverBot_Fingerii()
{
	Set_Name(TEXT("ReaverBot_Fingerii"));
	Set_RenderGroup(ERenderGroup::NonBlend);
	m_fHP = FGauge(10.f, true);
	Register_State();
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

CReaverBot_Fingerii::CReaverBot_Fingerii(const CReaverBot_Fingerii& rhs)
	: Base(rhs)
{
	NULL_CHECK(m_pModelComp = DynCast<CCommonModelComp*>(rhs.m_pModelComp->Clone()));
	Register_State();
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

HRESULT CReaverBot_Fingerii::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.2f, 0.2f, 0.2f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Fingerii/Fingerii.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Position(0.f, 2.f, 0.f);
	m_pColliderComp->Transform().Set_Scale(1.2f, 4.f, 1.2f);
	m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
	m_pColliderComp->EnterToPhysics(0);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR
		| COLLAYER_ATTACKER | COLLAYER_OBJECT);

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

HRESULT CReaverBot_Fingerii::Initialize_Prototype(FSerialData& InputData)
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

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			NULL_CHECK_RETURN(m_pModelComp = CCommonModelComp::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(TEXT("Model"), m_pModelComp)))
				return E_FAIL;
			break;
		case ECast(EComponentID::Collider):
			NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(TEXT("ColliderComp"), m_pColliderComp)))
				return E_FAIL;
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			m_pColliderComp->EnterToPhysics(0);
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

HRESULT CReaverBot_Fingerii::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CReaverBot_Fingerii::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

	return S_OK;
}

void CReaverBot_Fingerii::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Fingerii::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	Input_ActionKey();
	if (!m_State_Act.IsOnState(EState_Act::Dead) && m_fHP.Get_Percent() <= 0.f)
		m_State_Act.Set_State(EState_Act::Dead);

	m_State_AI.Get_StateFunc()(this, fTimeDelta);
	m_State_Act.Get_StateFunc()(this, fTimeDelta);
	m_ActionKey.Reset();

	m_pColliderComp->Tick(fTimeDelta);
}

void CReaverBot_Fingerii::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();
	m_pModelComp->Invalidate_BoneTransforms();

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Fingerii::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

CReaverBot_Fingerii* CReaverBot_Fingerii::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("ReaverBot_Fingerii Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CReaverBot_Fingerii* CReaverBot_Fingerii::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("ReaverBot_Fingerii Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CReaverBot_Fingerii::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("ReaverBot_Fingerii Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CReaverBot_Fingerii::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("ReaverBot_Fingerii Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Fingerii::Free()
{
	SUPER::Free();
}

FSerialData CReaverBot_Fingerii::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("HP", m_fHP.fMax);

	return Data;
}

FSerialData CReaverBot_Fingerii::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("HP", m_fHP.fMax);

	return Data;
}

HRESULT CReaverBot_Fingerii::Initialize_Component()
{
	

	return S_OK;
}

HRESULT CReaverBot_Fingerii::Initialize_Component(FSerialData& InputData)
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

		string strName = "";
		if (FAILED(InputProto.Get_Data("ProtoName", strName)))
			return E_FAIL;

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			NULL_CHECK_RETURN(m_pModelComp
				= DynCast<CCommonModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strName), VPCast(&InputProto))), E_FAIL);
			break;
		case ECast(EComponentID::Collider):
			NULL_CHECK_RETURN(m_pColliderComp
				= DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strName), VPCast(&InputProto))), E_FAIL);
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_ENEMY);

	return S_OK;
}

void CReaverBot_Fingerii::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);
}

void CReaverBot_Fingerii::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);
}

void CReaverBot_Fingerii::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);
}

void CReaverBot_Fingerii::Input_ActionKey()
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

	if (GI()->IsKey_Pressing(DIK_J))
		m_ActionKey.Act(EActionKey::Barrier);
}

void CReaverBot_Fingerii::Move_Update(const _float& fTimeDelta)
{
	if (m_ActionKey.IsAct(EActionKey::MoveForward))
		Transform().MoveForward(m_fMoveSpeed * fTimeDelta);
	else if (m_ActionKey.IsAct(EActionKey::MoveBackward))
		Transform().MoveForward(-m_fMoveSpeed * fTimeDelta);

	if (m_ActionKey.IsAct(EActionKey::TurnRight))
		Transform().TurnRight(1.f * fTimeDelta);
	else if (m_ActionKey.IsAct(EActionKey::TurnLeft))
		Transform().TurnRight(-1.f * fTimeDelta);

	if (m_ActionKey.IsAct(EActionKey::LookTarget)
		&& nullptr != m_pTarget)
		Transform().Look_At_OnLand(m_pTarget->Transform().Get_PositionVector(), 2.f * fTimeDelta);
}

void CReaverBot_Fingerii::Dead_Effect()
{
	CEffect_Common* pEffect = CEffect_Common::Create();
	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	if (nullptr == pEffect)
		return;

	pEffect->Transform().Set_Position(Transform().Get_PositionFloat3());
}

void CReaverBot_Fingerii::Register_State()
{
	for (_uint i = 0; i < ECast(EActionKey::Size); i++)
		m_ActionKey.Add_Action(Cast<EActionKey>(i));

	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::Walk, &ThisClass::ActState_Walk);
	m_State_Act.Add_Func(EState_Act::Barrier, &ThisClass::ActState_Walk);
	m_State_Act.Add_Func(EState_Act::Dead, &ThisClass::ActState_Dead);
	m_State_Act.Set_State(EState_Act::Idle);

	m_State_AI.Add_Func(EState_AI::Idle, &ThisClass::AIState_Idle);
	m_State_AI.Add_Func(EState_AI::Chase, &ThisClass::AIState_Chase);
	m_State_AI.Add_Func(EState_AI::Barrier, &ThisClass::AIState_Barrier);
	m_State_AI.Add_Func(EState_AI::Prowl, &ThisClass::AIState_Prowl);
	m_State_AI.Add_Func(EState_AI::Dead, &ThisClass::AIState_Dead);
	m_State_AI.Add_Func(EState_AI::Escape, &ThisClass::AIState_Escape);
	m_State_AI.Set_State(EState_AI::Idle);
}

void CReaverBot_Fingerii::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_fMoveSpeed = 3.f;
		m_pModelComp->Set_Animation(0, 1.f, true);
	}

	if (m_State_Act.Can_Update())
	{

		// 돌진
		if (m_ActionKey.IsAct(EActionKey::Barrier))
			m_State_Act.Set_State(EState_Act::Barrier);
		// 움직임
		if (m_ActionKey.IsAct(EActionKey::MoveForward) || m_ActionKey.IsAct(EActionKey::MoveBackward)
			|| m_ActionKey.IsAct(EActionKey::TurnRight) || m_ActionKey.IsAct(EActionKey::TurnLeft)
			|| m_ActionKey.IsAct(EActionKey::LookTarget))
			m_State_Act.Set_State(EState_Act::Walk);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CReaverBot_Fingerii::ActState_Walk(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(1, 1.3f, true);
	}

	if (m_State_Act.Can_Update())
	{
		Move_Update(fTimeDelta);

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

void CReaverBot_Fingerii::ActState_Barrier(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(2, 1.0f, true);
	}

	if (m_State_Act.Can_Update())
	{
		// 움직임
		if (!m_ActionKey.IsAct(EActionKey::Barrier))
			m_State_Act.Set_State(EState_Act::Idle);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}



void CReaverBot_Fingerii::ActState_Dead(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(2, 2.f, true);
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

void CReaverBot_Fingerii::AIState_Idle(const _float& fTimeDelta)
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

void CReaverBot_Fingerii::AIState_Chase(const _float& fTimeDelta)
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
				m_State_AI.Set_State(EState_AI::Barrier);
			}
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Fingerii::AIState_Barrier(const _float& fTimeDelta)
{
	if (m_State_AI.IsState_Entered())
	{
		m_fIdleTime.Readjust(2.5f);
	}

	if (m_State_AI.Can_Update())
	{
		// 사정거리 안에 들어온 적을 바라보며 기를 모은다.
		m_ActionKey.Act(EActionKey::Barrier);

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
			m_State_AI.Set_State(EState_AI::Idle);
		}
	}

	if (m_State_AI.IsState_Exit())
	{

	}
}

void CReaverBot_Fingerii::AIState_Prowl(const _float& fTimeDelta)
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

void CReaverBot_Fingerii::AIState_Dead(const _float& fTimeDelta)
{
}

void CReaverBot_Fingerii::AIState_Escape(const _float& fTimeDelta)
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

CCharacter_Common* CReaverBot_Fingerii::Find_Target()
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
