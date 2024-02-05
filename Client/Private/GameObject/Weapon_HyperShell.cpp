#include "GameObject/Weapon_HyperShell.h"

#include "Component/EffectComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/DamageCollision.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"


CWeapon_HyperShell::CWeapon_HyperShell()
{
	Set_Name(TEXT("Weapon_HyperShell"));
}

CWeapon_HyperShell::CWeapon_HyperShell(const CWeapon_HyperShell& rhs)
{
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

	m_fSpeed += 30.f * fTimeDelta;
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

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vColor = {
			1.0f, 0.7f, 0.2f, 1.f
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

void CWeapon_HyperShell::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
	m_RandomNumber = mt19937_64(m_RandomDevice());

	if (m_pModelComp)
	{
		m_pModelComp->Reset_ActivePass();
		m_pModelComp->Set_ActivePass(2);
	}
}

void CWeapon_HyperShell::BeginPlay()
{
	SUPER::BeginPlay();

	m_pColliderComp->EnterToPhysics(0);
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
	for(_uint i = 0; i < 20; ++i)
	{
		CDamageCollision* pEffect = CDamageCollision::Create();
		if (FAILED(GI()->Add_GameObject(pEffect)))
			return;

		if (nullptr == pEffect)
			return;

		uniform_real_distribution<_float> RandomTime(0.f, 0.25f);
		uniform_real_distribution<_float> RandomRadius(i * 0.5f, (i + 1.f) * 0.5f);
		uniform_real_distribution<_float> RandomRadius2(-1.f, 5.0f);
		uniform_real_distribution<_float> RandomPosY(-2.0f, 4.f);
		uniform_real_distribution<_float> RandomAngle(0.f, 360.0f);
		uniform_real_distribution<_float> RandomAngleZ(0.f, 180.0f);

		_vector vPos = XMVectorSet(0.f, 0.f, 1.f * RandomRadius(m_RandomNumber), 1.f);
		_vector vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_matrix RotationMatrix = XMMatrixRotationAxis(vAxisY, XMConvertToRadians(RandomAngle(m_RandomNumber)));
		vPos = XMVector3TransformCoord(vPos, RotationMatrix);

		_vector vPosY = XMVectorSet(1.f * RandomRadius2(m_RandomNumber), 0.f, 0.f, 1.f);
		_vector vAxisZ = XMVectorSet(0.f, 0.f, 1.f, 0.f);
		_matrix RotationZMatrix = XMMatrixRotationAxis(vAxisZ, XMConvertToRadians(RandomAngleZ(m_RandomNumber)));
		vPosY = XMVector3TransformCoord(vPosY, RotationZMatrix);

		pEffect->Transform().Set_Position(Transform().Get_PositionVector()
			+ XMVectorSet(XMVectorGetX(vPos), XMVectorGetY(vPosY), XMVectorGetZ(vPos), 0.f));
		pEffect->Transform().Set_Scale(3.f, 3.f, 3.f);
		pEffect->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
		pEffect->Set_LifeTime(0.7f);
		pEffect->Set_EffectCreateTime(RandomTime(m_RandomNumber) + i * 0.01f);
	}
}
