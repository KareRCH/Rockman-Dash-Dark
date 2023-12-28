#include "GameObject/Weapon_Buster.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"


CWeapon_Buster::CWeapon_Buster()
{
	Set_Name(TEXT("Weapon_Buster"));
}

CWeapon_Buster::CWeapon_Buster(const CWeapon_Buster& rhs)
{
}

HRESULT CWeapon_Buster::Initialize_Prototype()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	return S_OK;
}

HRESULT CWeapon_Buster::Initialize_Prototype(const _float3 vPos)
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CWeapon_Buster::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CWeapon_Buster::Initialize(const _float3 vPos)
{
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

	Transform().MoveForward(m_fSpeed * fTimeDelta);

	_vector vLook = XMVector3Normalize(Transform().Get_LookVector());
	_float3 vfLook = {};
	XMStoreFloat3(&vfLook, vLook);

	Transform().TurnAxis(vfLook, XMConvertToRadians(90.f * fTimeDelta));
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

	return S_OK;
}
