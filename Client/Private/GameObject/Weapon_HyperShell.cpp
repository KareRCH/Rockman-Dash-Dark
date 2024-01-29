#include "GameObject/Weapon_HyperShell.h"

#include "Component/EffectComponent.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Explosion.h"
#include "GameObject/LoadingScreen.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"


CWeapon_HyperShell::CWeapon_HyperShell()
{
	Set_Name(TEXT("Weapon_HyperShell"));
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

CWeapon_HyperShell::CWeapon_HyperShell(const CWeapon_HyperShell& rhs)
{
	m_RandomNumber = mt19937_64(m_RandomDevice());
}

HRESULT CWeapon_HyperShell::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_HyperShell::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_HyperShell::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_HyperShell::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_HyperShell::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_HyperShell::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		Create_Effect();
		return;
	}


	Transform().MoveForward(m_fSpeed * fTimeDelta);
	Transform().TurnLook(XMConvertToRadians(360.f * 10.f * fTimeDelta));

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_HyperShell::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_HyperShell::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

CWeapon_HyperShell* CWeapon_HyperShell::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_HyperShell* CWeapon_HyperShell::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_HyperShell::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_HyperShell::Free()
{
	SUPER::Free();
}

FSerialData CWeapon_HyperShell::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_HyperShell::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/BusterCannon.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
	m_pColliderComp->Set_CollisionKinematic();
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);
	m_pColliderComp->EnterToPhysics(0);


	return S_OK;
}

void CWeapon_HyperShell::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

}

void CWeapon_HyperShell::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			pEnemy->Damage_HP(1.f);
			Create_Effect();
			Set_Dead();
		}
	}

	CStaticObject* pSolid = DynCast<CStaticObject*>(pDst);
	if (nullptr != pSolid)
	{
		Create_Effect();
		Set_Dead();
	}

	CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
	if (nullptr != pDoor)
	{
		Create_Effect();
		Set_Dead();
	}
}


void CWeapon_HyperShell::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}

void CWeapon_HyperShell::Create_Effect()
{
	for(_uint i = 0; i < 5; ++i)
	{
		CEffect_Explosion* pEffect = CEffect_Explosion::Create();
		if (FAILED(GI()->Add_GameObject(pEffect)))
			return;

		if (nullptr == pEffect)
			return;


		uniform_real_distribution<_float> RandomTime(0.f, 0.25f);
		uniform_real_distribution<_float> RandomPosX(-3.f, 3.f);
		uniform_real_distribution<_float> RandomPosY(-2.f, 2.f);
		uniform_real_distribution<_float> RandomPosZ(-3.f, 3.f);

		pEffect->Transform().Set_Position(Transform().Get_PositionVector()
			+ XMVectorSet(RandomPosX(m_RandomNumber), RandomPosY(m_RandomNumber), RandomPosZ(m_RandomNumber), 0.f));
		pEffect->Set_StartTime(RandomTime(m_RandomNumber) + i * 0.05f);
	}
}
