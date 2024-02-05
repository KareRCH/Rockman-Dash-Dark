#include "GameObject/Weapon_Machinegun.h"

#include "Component/EffectComponent.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"


#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"


CWeapon_Machinegun::CWeapon_Machinegun()
{
	Set_Name(TEXT("Weapon_Machinegun"));
}

CWeapon_Machinegun::CWeapon_Machinegun(const CWeapon_Machinegun& rhs)
{
}

HRESULT CWeapon_Machinegun::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Machinegun::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_Machinegun::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_Machinegun::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_Machinegun::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_Machinegun::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		return;
	}


	Transform().MoveForward(m_fSpeed * fTimeDelta);
	Transform().TurnLook(XMConvertToRadians(360.f * 10.f * fTimeDelta));

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_Machinegun::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_Machinegun::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vColor = {
			1.0f, 1.f, 0.f, 1.f
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

void CWeapon_Machinegun::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
}

void CWeapon_Machinegun::BeginPlay()
{
	SUPER::BeginPlay();

	m_pColliderComp->EnterToPhysics(0);

	if (m_pModelComp)
	{
		m_pModelComp->Reset_ActivePass();
		m_pModelComp->Set_ActivePass(2);
	}
}

CWeapon_Machinegun* CWeapon_Machinegun::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_Machinegun* CWeapon_Machinegun::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Machinegun::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Machinegun Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_Machinegun::Free()
{
	SUPER::Free();
}

FSerialData CWeapon_Machinegun::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_Machinegun::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/MachinegunBullet.amodel");

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

void CWeapon_Machinegun::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

}

void CWeapon_Machinegun::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
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


void CWeapon_Machinegun::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}

void CWeapon_Machinegun::Create_Effect()
{
	CEffect_Common* pEffect = CEffect_Common::Create();
	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	if (nullptr == pEffect)
		return;

	//CPlaneModelComp* pModel = pEffect->Get_Component<CPlaneModelComp>(TEXT("PlaneComp"));
	pEffect->Transform().Set_Position(Transform().Get_PositionFloat3());
}
