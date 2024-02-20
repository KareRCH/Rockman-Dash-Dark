#include "GameObject/Weapon_Blade.h"

#include "Component/EffectComponent.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"
#include "GameObject/UI_Blink.h"
#include "GameObject/Effect_Particle.h"


CWeapon_Blade::CWeapon_Blade()
{
	Set_Name(TEXT("Weapon_Blade"));
}

CWeapon_Blade::CWeapon_Blade(const CWeapon_Blade& rhs)
{
}

HRESULT CWeapon_Blade::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Blade::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_Blade::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_Blade::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_Blade::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_Blade::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fColor.Increase(360.f * 5.f * fTimeDelta))
		m_fColor.Reset();

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_Blade::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();
	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_Blade::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vHSV = {
			m_fColor.fCur / 360.f,
			0.7f, 1.f, 1.f
		};
		
		_vector vSimRGB = XMColorHSVToRGB(XMLoadFloat4(&vHSV));
		XMStoreFloat4(&vHSV, vSimRGB);
		

		pEffectComp->Bind_RawValue("g_vColorAdd", VPCast(&vHSV), sizeof(_float4));
		//pEffectComp->Bind_RawValue("g_fColorAdd_Strength", VPCast(&fStrength), sizeof(_float));

		m_pModelComp->Render();
	}

#ifdef _DEBUG
	m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

void CWeapon_Blade::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
}

void CWeapon_Blade::BeginPlay()
{
	SUPER::BeginPlay();

	m_pColliderComp->EnterToPhysics(0);
}

CWeapon_Blade* CWeapon_Blade::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_Blade* CWeapon_Blade::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Blade::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_Blade::Free()
{
	SUPER::Free();
}

FSerialData CWeapon_Blade::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_Blade::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(2.0f, 8.0f, 2.0f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/Blade.amodel");
	m_pModelComp->Reset_ActivePass();
	m_pModelComp->Set_ActivePass(4);
	m_pModelComp->Set_Animation(0, 1.f, false);

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
	m_pColliderComp->Transform().Set_PositionY(12.f);
	m_pColliderComp->Transform().Set_ScaleY(50.f);
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
	m_pColliderComp->Set_CollisionKinematic();
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);
	
	return S_OK;
}

void CWeapon_Blade::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

}

void CWeapon_Blade::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			pEnemy->Damage_HP(6.f);
			m_pGI->Add_GameObject(CUI_Blink::Create());
			m_pGI->Play_Sound(TEXT("RockmanDash2"), TEXT("explosion_small.mp3"), CHANNELID::SOUND_PLAYER3, 1.f);

			auto pParticle = CEffect_Particle::Create();
			m_pGI->Add_GameObject(pParticle);

			CEffect_Particle::TParticleDesc Desc = {};
			Desc.InstancingDesc = {
				{ 0.f, 0.f, 0.f },	// Center
				{ 0.5f },			// Range
				{ 10.3f, 15.3f },		// Speed
				{ 0.05f, 0.15f },	// Scale
				{ 0.5f, 1.0f }		// LifeTime
			};
			Desc.iNumInstances = 20;
			Desc.strTexturePath = TEXT("Textures/RockmanDash2/Effects/CommonExplosion/Explosion%d.png");
			Desc.iNumTextures = 8;

			_float4 vHSV = {
			m_fColor.fCur / 360.f,
			0.7f, 1.f, 1.f
			};

			_vector vSimRGB = XMColorHSVToRGB(XMLoadFloat4(&vHSV));
			XMStoreFloat4(&vHSV, vSimRGB);

			Desc.vBlendColor = vHSV;
			Desc.fBlendStrength = 0.3f;

			pParticle->Create_Instancing(Desc);
			pParticle->Transform().Set_Position(pEnemy->Transform().Get_PositionFloat3());
		}
	}
}


void CWeapon_Blade::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}
