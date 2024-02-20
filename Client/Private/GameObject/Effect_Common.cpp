#include "GameObject/Effect_Common.h"

#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CEffect_Common::CEffect_Common()
{
	Set_Name(TEXT("Effect_Common"));
	Set_RenderGroup(ERenderGroup::Blend);
}

CEffect_Common::CEffect_Common(const CEffect_Common& rhs)
	: Base(rhs)
{
}

HRESULT CEffect_Common::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

	return S_OK;
}

HRESULT CEffect_Common::Initialize(void* Arg)
{


	return S_OK;
}

void CEffect_Common::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CEffect_Common::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	Transform().Look_At(PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One));
	if (m_fLifeTime.Increase(fTimeDelta))
		Set_Dead();

	if (m_fAnimTime.Increase(m_fAnimSpeed * fTimeDelta))
		m_fAnimTime.Reset();

	m_pPlaneModelComp->Set_CurrentTextureIndex(Cast<_uint>(m_fAnimTime.fCur));
	m_pPlaneModelComp->Set_Alpha(max(1.f + (m_fAlphaMinTime - m_fLifeTime.Get_Percent()) * 3.f, 0.f));
}

void CEffect_Common::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CEffect_Common::Render()
{
	SUPER::Render();

	m_pPlaneModelComp->Render();

	return S_OK;
}

CEffect_Common* CEffect_Common::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Effect_Common Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Common::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Effect_Common Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CEffect_Common::Free()
{
	SUPER::Free();
}

FSerialData CEffect_Common::SerializeData()
{
	return FSerialData();
}

HRESULT CEffect_Common::Initialize_Component()
{
	// 프로토타입용 컴포넌트 초기화
	if (FAILED(Add_Component(L"PlaneComp", m_pPlaneModelComp = CPlaneModelComp::Create())))
		return E_FAIL;

	m_fAnimSpeed = 10.f;
	m_fAnimTime = FGauge(8.f);
	m_fLifeTime = FGauge(1.f);

	m_pPlaneModelComp->Set_Mode(CPlaneModelComp::PERSP);
	m_pPlaneModelComp->Transform().Set_Scale(1.3f, 1.3f, 1.3f);
	m_pPlaneModelComp->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/Effects/CommonExplosion/Explosion%d.png"), 8);
	m_pPlaneModelComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);

	return S_OK;
}
