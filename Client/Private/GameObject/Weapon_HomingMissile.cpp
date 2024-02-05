#include "GameObject/Weapon_HomingMissile.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"
#include "Component/EffectComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"
#include "GameObject/Effect_Explosion.h"
#include "GameObject/DamageCollision.h"


CWeapon_HomingMissile::CWeapon_HomingMissile()
{
	Set_Name(TEXT("Weapon_HomingMissile"));
}

CWeapon_HomingMissile::CWeapon_HomingMissile(const CWeapon_HomingMissile& rhs)
{
}

HRESULT CWeapon_HomingMissile::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_HomingMissile::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_HomingMissile::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_HomingMissile::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_HomingMissile::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_HomingMissile::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		Create_Effect();
		return;
	}

	if (nullptr == m_pTarget)
	{
		auto pTarget = Find_Target(20.f);
		if (nullptr != pTarget)
			Set_Target(pTarget);
	}

	if (nullptr != m_pTarget)
	{
		if (m_pTarget->IsDead())
		{
			Safe_ReleaseAndUnlink(m_pTarget);
		}
		else
		{
			_matrix ColliderMatrix = m_pTarget->ColliderComp()->Calculate_TransformMatrixFromParent();
			Transform().Look_At(ColliderMatrix.r[3], fTimeDelta);
		}
	}

	Transform().MoveForward(m_fSpeed * fTimeDelta);
	//Transform().TurnRight(XMConvertToRadians(360.f * 10.f * fTimeDelta));

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_HomingMissile::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_HomingMissile::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vColor = {
			1.0f, 0.8f, 0.3f, 1.f
		};

		pEffectComp->Bind_RawValue("g_vColorAdd", VPCast(&vColor), sizeof(_float4));
		//pEffectComp->Bind_RawValue("g_fColorAdd_Strength", VPCast(&fStrength), sizeof(_float));

		m_pModelComp->Render();
	}

#ifdef _DEBUG
	m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

void CWeapon_HomingMissile::OnCreated()
{
	Set_RenderGroup(ERenderGroup::Blend);
}

void CWeapon_HomingMissile::BeginPlay()
{
	if (m_pModelComp)
	{
		m_pModelComp->Reset_ActivePass();
		m_pModelComp->Set_ActivePass(2);
	}
}

CWeapon_HomingMissile* CWeapon_HomingMissile::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_HomingMissile Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_HomingMissile* CWeapon_HomingMissile::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_HomingMissile Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_HomingMissile::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_HomingMissile Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_HomingMissile::Free()
{
	SUPER::Free();
	
	Safe_Release(m_pTarget);
}

FSerialData CWeapon_HomingMissile::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_HomingMissile::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/Missile.amodel");

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

void CWeapon_HomingMissile::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

}

void CWeapon_HomingMissile::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
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


void CWeapon_HomingMissile::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}

void CWeapon_HomingMissile::Create_Effect()
{
	CDamageCollision* pEffect = CDamageCollision::Create();
	if (nullptr == pEffect)
		return;

	pEffect->Transform().Set_Position(Transform().Get_PositionFloat3());
	pEffect->Transform().Set_Scale(3.f, 3.f, 3.f);

	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	pEffect->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
}

CCharacter_Common* CWeapon_HomingMissile::Find_Target(_float fRange)
{
	auto listObjects = GI()->IntersectTests_Sphere_GetGameObject(0, Transform().Get_PositionFloat3(), fRange, COLLAYER_CHARACTER);
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
				_float fDot = XMVector3Dot(XMVector3Normalize(pObj->Transform().Get_PositionVector() - Transform().Get_PositionVector()),
					Transform().Get_LookNormalizedVector()).m128_f32[0];
				_float fDir = XMVectorGetX(XMVector3Dot(Transform().Get_RightNormalizedVector(), Transform().Get_LookNormalizedVector()));
				_float fRadian = acosf(fDot);
				if (isnan(fRadian))
					fRadian = 0.f;

				if (fRadian < XMConvertToRadians(45.f) && fDir >= 0)
				{
					fDistance = fObjDistance;
					pClosestObj = pObj;
				}
			}
		}
	}

	return DynCast<CCharacter_Common*>(pClosestObj);
}