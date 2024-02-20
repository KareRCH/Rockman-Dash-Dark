#include "GameObject/Weapon_BusterCannon.h"

#include "Component/EffectComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"
#include "GameObject/Effect_Particle.h"


CWeapon_BusterCannon::CWeapon_BusterCannon()
{
	Set_Name(TEXT("Weapon_Buster"));
}

CWeapon_BusterCannon::CWeapon_BusterCannon(const CWeapon_BusterCannon& rhs)
{
}

HRESULT CWeapon_BusterCannon::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_BusterCannon::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_BusterCannon::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_BusterCannon::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_BusterCannon::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_BusterCannon::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		return;
	}

	if (m_fParticleTime.Increase(fTimeDelta))
	{
		m_fParticleTime.Reset();

		auto pParticle = CEffect_Particle::Create();
		m_pGI->Add_GameObject(pParticle);

		CEffect_Particle::TParticleDesc Desc = {};
		Desc.InstancingDesc = {
			{ 0.f, 0.f, 0.f },	// Center
			{ 0.3f },			// Range
			{ 0.025f, 0.075f },	// Speed
			{ 0.025f, 0.075f },	// Scale
			{ 0.5f, 1.25f }		// LifeTime
		};
		Desc.iNumInstances = 10;
		Desc.strTexturePath = TEXT("Textures/RockmanDash2/Effects/CommonExplosion/Explosion%d.png");
		Desc.iNumTextures = 8;
		Desc.vBlendColor = { 0.f, 0.f, 1.f, 1.f };
		Desc.fBlendStrength = { 0.7f };

		pParticle->Create_Instancing(Desc);
		pParticle->Transform().Set_Position(Transform().Get_PositionFloat3());
	}

	_float3 vfLook = {};
	XMStoreFloat3(&vfLook, Transform().Get_LookNormalizedVector());

	Transform().MoveForward(m_fSpeed * fTimeDelta);
	Transform().TurnAxis(vfLook, XMConvertToRadians(90.f * fTimeDelta));

	if (m_pLight)
	{
		_float4 vPos = {};
		XMStoreFloat4(&vPos, Transform().Get_PositionVector());
		TLIGHT_DESC LightDesc = m_pLight->Get_Desc();
		LightDesc.vPosition = vPos;
		m_pLight->Update_Light(LightDesc);
	}

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_BusterCannon::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_BusterCannon::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vColor = {
			0.4f, 0.4f, 1.0f, 1.f
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

void CWeapon_BusterCannon::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
}

void CWeapon_BusterCannon::BeginPlay()
{
	SUPER::BeginPlay();

	if (m_pColliderComp)
		m_pColliderComp->EnterToPhysics(0);

	if (m_pModelComp)
	{
		m_pModelComp->Reset_ActivePass();
		m_pModelComp->Set_ActivePass(2);
	}


	TLIGHT_DESC LightDesc = {};
	LightDesc.eType = TLIGHT_DESC::TYPE_POINT;
	XMStoreFloat4(&LightDesc.vPosition, Transform().Get_PositionVector());
	LightDesc.fPower = 10.f;
	LightDesc.fRange = 0.8f;
	//LightDesc.fDamping = 1.f;
	LightDesc.vDiffuse = { 0.f, 1.f, 0.f, 1.f };
	LightDesc.vAmbient = { 0.f, 1.f, 0.f, 1.f };
	LightDesc.vSpecular = LightDesc.vDiffuse;

	m_pGI->Add_Light(LightDesc, m_iLightID, &m_pLight);
	Safe_AddRef(m_pLight);
}

void CWeapon_BusterCannon::EndPlay()
{
	SUPER::EndPlay();


}

CWeapon_BusterCannon* CWeapon_BusterCannon::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_BusterCannon* CWeapon_BusterCannon::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_BusterCannon::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_BusterCannon::Free()
{
	SUPER::Free();

	if (m_pLight)
	{
		m_pGI->Remove_Light(m_iLightID);
		Safe_Release(m_pLight);
	}
}

FSerialData CWeapon_BusterCannon::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_BusterCannon::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Buster/Buster.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
	m_pColliderComp->Set_CollisionKinematic();
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);

	return S_OK;
}

void CWeapon_BusterCannon::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

}

void CWeapon_BusterCannon::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			pEnemy->Damage_HP(m_fDamage);
			Create_Effect();
			Set_Dead();
			m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("buster_cannon_hit.mp3"), CHANNELID::SOUND_PLAYER3, 2.f);

			auto pParticle = CEffect_Particle::Create();
			m_pGI->Add_GameObject(pParticle);

			CEffect_Particle::TParticleDesc Desc = {};
			Desc.InstancingDesc = {
				{ 0.f, 0.f, 0.f },	// Center
				{ 1.5f },			// Range
				{ 10.f, 20.f },	// Speed
				{ 0.1f, 0.2f },	// Scale
				{ 0.1f, 0.25f }		// LifeTime
			};
			Desc.iNumInstances = 100;
			Desc.strTexturePath = TEXT("Textures/RockmanDash2/Effects/CommonExplosion/Explosion%d.png");
			Desc.iNumTextures = 8;
			Desc.vBlendColor = { 0.f, 0.f, 1.f, 1.f };
			Desc.fBlendStrength = { 0.7f };

			pParticle->Create_Instancing(Desc);
			pParticle->Transform().Set_Position(Transform().Get_PositionFloat3());
		}
	}

	CStaticObject* pSolid = DynCast<CStaticObject*>(pDst);
	if (nullptr != pSolid)
	{
		Create_Effect();
		Set_Dead();
		m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("buster_cannon_hit.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
	}

	CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
	if (nullptr != pDoor)
	{
		Create_Effect();
		Set_Dead();
		m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("buster_cannon_hit.mp3"), CHANNELID::SOUND_PLAYER2, 1.f);
	}
}


void CWeapon_BusterCannon::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}

void CWeapon_BusterCannon::Create_Effect()
{
	CEffect_Common* pEffect = CEffect_Common::Create();
	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	if (nullptr == pEffect)
		return;

	pEffect->Transform().Set_Position(Transform().Get_PositionFloat3());
}
