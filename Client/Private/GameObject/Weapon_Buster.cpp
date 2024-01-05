#include "GameObject/Weapon_Buster.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/LoadingScreen.h"


CWeapon_Buster::CWeapon_Buster()
{
	Set_Name(TEXT("Weapon_Buster"));
}

CWeapon_Buster::CWeapon_Buster(const CWeapon_Buster& rhs)
{
}

HRESULT CWeapon_Buster::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Buster::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_Buster::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_Buster::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_Buster::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_Buster::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
	{
		Set_Dead();
		return;
	}

	

	_vector vRight = {};
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vFlyingDir = XMLoadFloat3(&m_vLookDir);
	vFlyingDir = XMVector3Normalize(vFlyingDir);
	vRight = XMVector3Cross(vFlyingDir, vUp);
	vUp = XMVector3Cross(vRight, vFlyingDir);

	_vector vLook = XMVector3Normalize(Transform().Get_LookVector());
	_float3 vfLook = {};
	XMStoreFloat3(&vfLook, vLook);

	Transform().MoveForward(m_fSpeed * fTimeDelta);
	Transform().TurnAxis(vfLook, XMConvertToRadians(90.f * fTimeDelta));

	m_pColliderComp->Tick(fTimeDelta);
}

void CWeapon_Buster::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_Buster::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	m_pColliderComp->Render();
#endif

	return S_OK;
}

CWeapon_Buster* CWeapon_Buster::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_Buster* CWeapon_Buster::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_Buster::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_Buster::Free()
{
	SUPER::Free();
}

HRESULT CWeapon_Buster::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

	m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelNoAnim.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Buster/Buster.amodel");

	if (nullptr == m_pColliderComp)
		return E_FAIL;
	m_pColliderComp->Bind_Collision(ECollisionType::Sphere);
	m_pColliderComp->EnterToPhysics(0);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_ATTACKER);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER | COLLAYER_WALL | COLLAYER_FLOOR | COLLAYER_OBJECT);

	return S_OK;
}

void CWeapon_Buster::OnCollision(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollision(pDst, pContact);

	cout << "충돌함" << endl;
	
}

void CWeapon_Buster::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
	SUPER::OnCollisionEntered(pDst, pContact);

	cout << "충돌 진입" << endl;
	CCharacter_Common* pEnemy = DynCast<CCharacter_Common*>(pDst);
	if (pEnemy)
	{
		if (CTeamAgentComp::ERelation::Hostile == 
			CTeamAgentComp::Check_Relation(&TeamAgentComp(), &pEnemy->TeamAgentComp()))
		{
			Create_Effect();
			Set_Dead();
		}
	}
}


void CWeapon_Buster::OnCollisionExited(CGameObject* pDst)
{
	SUPER::OnCollisionExited(pDst);

	cout << "충돌 나감" << endl;
}

void CWeapon_Buster::Create_Effect()
{
	CEffect_Common* pEffect = CEffect_Common::Create();
	if (FAILED(GI()->Add_GameObject(pEffect)))
		return;

	if (nullptr == pEffect)
		return;

	//CPlaneModelComp* pModel = pEffect->Get_Component<CPlaneModelComp>(TEXT("PlaneComp"));
	pEffect->Transform().Set_Position(Transform().Get_PositionFloat3());
}
