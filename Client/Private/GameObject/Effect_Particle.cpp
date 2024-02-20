#include "GameObject/Effect_Particle.h"

#include "Component/InstancingModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CEffect_Particle::CEffect_Particle()
{
	Set_Name(TEXT("Effect_Common"));
}

CEffect_Particle::CEffect_Particle(const CEffect_Particle& rhs)
	: Base(rhs)
{
}

HRESULT CEffect_Particle::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::Initialize(void* Arg)
{


	return S_OK;
}

void CEffect_Particle::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CEffect_Particle::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (m_fLifeTime.Increase(fTimeDelta))
		Set_Dead();

	m_pInstancingComp->Tick(fTimeDelta);
}

void CEffect_Particle::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Particle::Render()
{
	SUPER::Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pInstancingComp->BindAndRender_Buffer(0)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Particle::OnCreated()
{
	SUPER::OnCreated();

	Set_RenderGroup(ERenderGroup::Blend);
}

CEffect_Particle* CEffect_Particle::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CEffect_Particle Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Particle::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CEffect_Particle Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CEffect_Particle::Free()
{
	SUPER::Free();
}

FSerialData CEffect_Particle::SerializeData_Prototype()
{
	return FSerialData();
}

FSerialData CEffect_Particle::SerializeData()
{
	return FSerialData();
}

HRESULT CEffect_Particle::Initialize_Component()
{
	// 프로토타입용 컴포넌트 초기화
	if (FAILED(Add_Component(L"InstancingComp", m_pInstancingComp = CInstancingModelComp::Create())))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::Bind_ShaderResources()
{
	auto pEffect = m_pInstancingComp->EffectComp();
	_float4x4 TempFloat4x4 = m_pInstancingComp->Calculate_TransformFloat4x4FromParent();
	_float4 TempFloat4 = {};

	if (FAILED(pEffect->Bind_Matrix("g_WorldMatrix", &TempFloat4x4)))
		return E_FAIL;
	if (FAILED(pEffect->Bind_Matrix("g_ViewMatrix", &(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(pEffect->Bind_Matrix("g_ProjMatrix", &(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(pEffect->Bind_RawValue("g_vCamPosition", 
		&(TempFloat4 = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One)), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pEffect->Bind_RawValue("g_vBlendColor",
		&(m_vBlendColor), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pEffect->Bind_RawValue("g_fBlendStrength",
		&(m_fBlendStrength), sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pInstancingComp->Bind_TextureToEffect("g_Texture", 1)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Particle::Create_Instancing(TParticleDesc Desc)
{
	if (nullptr == m_pInstancingComp)
		return E_FAIL;

	if (FAILED(m_pInstancingComp->Load_Texture(Desc.strTexturePath, Desc.iNumTextures)))
		return E_FAIL;
	if (FAILED(m_pInstancingComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_ParticlePoint.hlsl"),
		VTX_PARTICLE_POINT::Elements, VTX_PARTICLE_POINT::iNumElements)))
		return E_FAIL;
	if (FAILED(m_pInstancingComp->Create_InstanceVIBuffers(Desc.iNumInstances, Desc.InstancingDesc)))
		return E_FAIL;

	m_fLifeTime.Readjust(Desc.InstancingDesc.vLifeTime.y);

	m_vBlendColor = Desc.vBlendColor;
	m_fBlendStrength = Desc.fBlendStrength;

	return S_OK;
}
