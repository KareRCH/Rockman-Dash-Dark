#include "GameObject/Weapon_LaserEmission.h"

#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Effect_Common.h"
#include "GameObject/Weapon_Laser.h"


CWeapon_LaserEmission::CWeapon_LaserEmission()
{
	Set_Name(TEXT("Weapon_LaserEmission"));
	Set_RenderGroup(ERenderGroup::Blend);
	FAILED_CHECK_RETURN(Add_Component(TEXT("TeamAgentComp"),
		m_pTeamAgentComp = CTeamAgentComp::Create()), );
}

CWeapon_LaserEmission::CWeapon_LaserEmission(const CWeapon_LaserEmission& rhs)
{
	FAILED_CHECK_RETURN(Add_Component(TEXT("TeamAgentComp"),
		m_pTeamAgentComp = DynCast<CTeamAgentComp*>(rhs.m_pTeamAgentComp->Clone())), );
}

HRESULT CWeapon_LaserEmission::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_LaserEmission::Initialize_Prototype(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_LaserEmission::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

HRESULT CWeapon_LaserEmission::Initialize(const _float3 vPos)
{
	FAILED_CHECK_RETURN(__super::Initialize(), E_FAIL);

	Transform().Set_Position(vPos);

	return S_OK;
}

void CWeapon_LaserEmission::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CWeapon_LaserEmission::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);
	
	if (m_fLaserCreateTime.Increase(fTimeDelta))
	{
		Create_Laser();
	}

	if (nullptr != m_pLaser)
	{
		m_pLaser->Transform().Set_Position(Transform().Get_PositionVector());
		m_pLaser->Transform().Look_At(Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector());
	}
}

void CWeapon_LaserEmission::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CWeapon_LaserEmission::Render()
{
	SUPER::Render();

	// 기존 모델과 다르게 표현하기 위해 수동으로 바인드 한다.
	if (m_pModelComp)
	{
		_float4x4 TempFloat4x4 = m_pModelComp->Calculate_TransformFloat4x4FromParent();
		_float4 TempFloat4 = {};
		_float	TempFloat = {};

		auto pEffectComp = m_pModelComp->EffectComp();
		pEffectComp->Bind_Matrix("g_WorldMatrix", &TempFloat4x4);
		pEffectComp->Bind_Matrix("g_ViewMatrix", 
			&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		pEffectComp->Bind_Matrix("g_ProjMatrix", 
			&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));

		pEffectComp->Bind_RawValue("g_vCamPosition",
			VPCast(&(TempFloat4 = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One))), sizeof(_float4));

		pEffectComp->Bind_RawValue("g_vColor", VPCast(&(TempFloat4 = _float4(1.f, 1.f, 1.f, 1.f))), sizeof(_float4));

		pEffectComp->Bind_RawValue("g_fElipse", VPCast(&(TempFloat = _float(0.18f))), sizeof(_float));
		pEffectComp->Bind_RawValue("g_fAlpha", VPCast(&(TempFloat = _float(0.1f))), sizeof(_float));

		// 그리기 시작
		pEffectComp->Begin(0);

		// 버퍼를 장치에 바인드, 그리기
		m_pModelComp->BindAndRender_Mesh(0);
	}

	return S_OK;
}

CWeapon_LaserEmission* CWeapon_LaserEmission::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_LaserEmission Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CWeapon_LaserEmission* CWeapon_LaserEmission::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("Weapon_LaserEmission Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWeapon_LaserEmission::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_LaserEmission Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CWeapon_LaserEmission::Free()
{
	SUPER::Free();

	if (m_pLaser)
	{
		m_pLaser->Set_Dead();
	}
	Safe_Release(m_pLaser);
}

FSerialData CWeapon_LaserEmission::SerializeData()
{
	return FSerialData();
}

void CWeapon_LaserEmission::Create_Laser()
{
	if (nullptr != m_pLaser)
		return;

	_vector vPos = Transform().Get_PositionVector();
	_float3 vfPos = {};
	XMStoreFloat3(&vfPos, vPos);

	m_pLaser = CWeapon_Laser::Create(vfPos);
	if (m_pLaser == nullptr)
		return;

	m_pLaser->Transform().Look_At(m_pLaser->Transform().Get_PositionVector() + Transform().Get_LookNormalizedVector());
	m_pLaser->TeamAgentComp().Set_TeamID(TeamAgentComp().Get_TeamID());
	GI()->Add_GameObject(m_pLaser);
	Safe_AddRef(m_pLaser);
}

HRESULT CWeapon_LaserEmission::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationEulerX(XMConvertToRadians(-90.f));
	m_pModelComp->Transform().Set_RotationEulerY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.8f, 0.8f, 0.8f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ViewShape.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Character/RockVolnutt/Weapons/LaserSphere.amodel");

	return S_OK;
}
