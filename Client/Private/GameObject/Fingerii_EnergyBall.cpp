#include "GameObject/Fingerii_EnergyBall.h"

#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"


CFingerii_EnergyBall::CFingerii_EnergyBall()
{
	Set_Name(TEXT("Fingerii_EnergyBall"));
}

CFingerii_EnergyBall::CFingerii_EnergyBall(const CFingerii_EnergyBall& rhs)
{
}

HRESULT CFingerii_EnergyBall::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFingerii_EnergyBall::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CFingerii_EnergyBall::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CFingerii_EnergyBall::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CFingerii_EnergyBall::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CFingerii_EnergyBall::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		return;
	}

	if (nullptr != m_pTarget)
	{
		Transform().MoveForward(fTimeDelta * 1.5f);
		_matrix TargetMatrix = m_pTarget->ColliderComp()->Calculate_TransformMatrixFromParent();
		Transform().Look_At(TargetMatrix.r[3], fTimeDelta);
	}

	m_pColliderComp->Tick(fTimeDelta);
}

void CFingerii_EnergyBall::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CFingerii_EnergyBall::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

CFingerii_EnergyBall* CFingerii_EnergyBall::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fingerii_EnergyBall Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CFingerii_EnergyBall* CFingerii_EnergyBall::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Fingerii_EnergyBall Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFingerii_EnergyBall::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Fingerii_EnergyBall Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CFingerii_EnergyBall::Free()
{
	SUPER::Free();

	Safe_Release(m_pTarget);
}

FSerialData CFingerii_EnergyBall::SerializeData()
{
	return FSerialData();
}

HRESULT CFingerii_EnergyBall::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Buster/Buster.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
	m_pColliderComp->Set_CollisionKinematic();
	m_pColliderComp->EnterToPhysics(0);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER);

	return S_OK;
}

void CFingerii_EnergyBall::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	//SUPER::OnCollision(pDst, pContact);

}

void CFingerii_EnergyBall::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			pEnemy->Damage_HP(5.f);
		}
	}
}


void CFingerii_EnergyBall::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}

void CFingerii_EnergyBall::Set_Target(CCharacter_Common* pTarget)
{
	m_pTarget = pTarget;
	Safe_AddRef(m_pTarget);
}
