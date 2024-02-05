#include "GameObject/Weapon_Laser.h"


#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"
#include "Component/EffectComponent.h"


#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"
#include "GameObject/StaticObject.h"
#include "GameObject/Door_Common.h"


CWeapon_Laser::CWeapon_Laser()
{
	Set_Name(TEXT("Weapon_Laser"));
}

CWeapon_Laser::CWeapon_Laser(const CWeapon_Laser& rhs)
{
}

HRESULT CWeapon_Laser::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Laser::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_Laser::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_Laser::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_Laser::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_Laser::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	_float fRange = m_fRange;
	_float fCoefficient = m_pModelComp->Transform().Get_ScaleFloat3().x;
	_vector vPos = Transform().Get_PositionVector();
	if (m_bIsContact)
	{
		fRange = XMVector3Length(vPos - XMLoadFloat3(&m_vContactPoint)).m128_f32[0];
	}
	
	//fRange /= fCoefficient;
	Transform().Set_ScaleZ(fRange);
	m_bIsContact = false;

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_Laser::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_Laser::Render()
{
	SUPER::Render();

	if (m_pModelComp)
	{
		auto pEffectComp = m_pModelComp->EffectComp();
		_float fStrength = 1.f;

		_float4 vColor = {
			0.55f, 1.f, 0.55f, 1.f
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

void CWeapon_Laser::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
}

void CWeapon_Laser::BeginPlay()
{
	SUPER::BeginPlay();

	m_pColliderComp->EnterToPhysics(0);

	if (m_pModelComp)
	{
		m_pModelComp->Reset_ActivePass();
		m_pModelComp->Set_ActivePass(2);
	}
}

CWeapon_Laser* CWeapon_Laser::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_Laser* CWeapon_Laser::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Laser::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_Laser::Free()
{
	SUPER::Free();
}

FSerialData CWeapon_Laser::SerializeData()
{
	return FSerialData();
}

HRESULT CWeapon_Laser::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.25f, 1.f, 0.25f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/Laser.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pColliderComp->Transform().Set_PositionZ(0.5f);
	m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
	m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
	m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
	m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
	m_pColliderComp->Set_CollisionKinematic();
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);

	return S_OK;
}

void CWeapon_Laser::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	//SUPER::OnCollision(pDst, pContact);

	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile ==
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			pEnemy->Damage_HP(0.01f);
			_float fLength = XMVector3Length(XMLoadFloat3(&m_vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
			_float fNewLength = XMVector3Length(XMLoadFloat3(&pContact->vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
			if (fNewLength < fLength)
				m_vContactPoint = pContact->vContactPoint;
			m_bIsContact = true;
		}
	}

	CStaticObject* pSolid = DynCast<CStaticObject*>(pDst);
	if (nullptr != pSolid)
	{
		_float fLength = XMVector3Length(XMLoadFloat3(&m_vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
		_float fNewLength = XMVector3Length(XMLoadFloat3(&pContact->vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
		if (fNewLength < fLength)
			m_vContactPoint = pContact->vContactPoint;
		m_bIsContact = true;
	}

	CDoor_Common* pDoor = DynCast<CDoor_Common*>(pDst);
	if (nullptr != pDoor)
	{
		_float fLength = XMVector3Length(XMLoadFloat3(&m_vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
		_float fNewLength = XMVector3Length(XMLoadFloat3(&pContact->vContactPoint) - Transform().Get_PositionVector()).m128_f32[0];
		if (fNewLength < fLength)
			m_vContactPoint = pContact->vContactPoint;
		m_bIsContact = true;
	}
}

void CWeapon_Laser::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	
}


void CWeapon_Laser::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

}
