#include "GameObject/Effect_Explosion.h"

#include "Component/EffectComponent.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"


CEffect_Explosion::CEffect_Explosion()
{
	Set_Name(TEXT("Effect_Explosion"));
	Set_RenderGroup(ERenderGroup::Blend);
}

CEffect_Explosion::CEffect_Explosion(const CEffect_Explosion& rhs)
{
}

HRESULT CEffect_Explosion::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(), E_FAIL);
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Explosion::Initialize(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize(Arg), E_FAIL);

	return S_OK;
}

void CEffect_Explosion::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CEffect_Explosion::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fStartTime.Increase(fTimeDelta))
	{
		if (m_fLifeTime.Increase(fTimeDelta))
		{
			Set_Dead();
			return;
		}

		m_fOffset += 18.f * fTimeDelta;
		if (m_fOffset > 7.f)
			m_fOffset = 0.f;

		m_fAlpha.Decrease(fTimeDelta * 3.f);
		m_fScale.Increase(2500.f * fTimeDelta);

		Transform().Set_Scale(_float3(m_fScale.fCur, m_fScale.fCur, m_fScale.fCur));
	}
}

void CEffect_Explosion::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CEffect_Explosion::Render()
{
	SUPER::Render();

	if (m_pModelComp && m_fStartTime.IsMax())
	{
		_float4x4 TempFloat4x4 = m_pModelComp->Calculate_TransformFloat4x4FromParent();
		_float4 TempFloat4 = {};
		_float	TempFloat = {};
		auto pEffectComp = m_pModelComp->EffectComp();

		

		//{
		//	pEffectComp->Bind_Matrix("g_WorldMatrix", &TempFloat4x4);
		//	pEffectComp->Bind_Matrix("g_ViewMatrix",
		//		&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		//	pEffectComp->Bind_Matrix("g_ProjMatrix",
		//		&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));
		//
		//	m_pModelComp->Bind_MeshMaterial(1, aiTextureType_DIFFUSE, "g_DiffuseTexture");
		//	pEffectComp->Bind_RawValue("g_fAlpha", VPCast(&m_fAlpha.fCur), sizeof(_float));
		//
		//	// 그리기 시작
		//	pEffectComp->Begin(1);
		//
		//	// 버퍼를 장치에 바인드, 그리기
		//	m_pModelComp->BindAndRender_Mesh(1);
		//}

		{
			pEffectComp->Bind_Matrix("g_WorldMatrix", &TempFloat4x4);
			pEffectComp->Bind_Matrix("g_ViewMatrix",
				&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
			pEffectComp->Bind_Matrix("g_ProjMatrix",
				&(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));

			m_pModelComp->Bind_MeshMaterial(0, aiTextureType_DIFFUSE, "g_DiffuseTexture");
			pEffectComp->Bind_RawValue("g_fOffset", VPCast(&m_fOffset), sizeof(_float));
			pEffectComp->Bind_RawValue("g_fAlpha", VPCast(&m_fAlpha.fCur), sizeof(_float));

			// 그리기 시작
			pEffectComp->Begin(0);

			// 버퍼를 장치에 바인드, 그리기
			m_pModelComp->BindAndRender_Mesh(0);
		}
	}

	return S_OK;
}

CEffect_Explosion* CEffect_Explosion::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Explosion::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Weapon_Buster Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CEffect_Explosion::Free()
{
	SUPER::Free();
}

FSerialData CEffect_Explosion::SerializeData()
{
	return FSerialData();
}

HRESULT CEffect_Explosion::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_RotationFixedX(XMConvertToRadians(90.f));
	m_pModelComp->Transform().Set_RotationFixedY(XMConvertToRadians(180.f));
	m_pModelComp->Transform().Set_Scale(_float3(0.01f, 0.01f, 0.01f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ExplosionLine.hlsl", SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_NONANIM, EModelGroupIndex::Permanent, L"Model/Effect/Explosion/Explosion.amodel");

	return S_OK;
}
