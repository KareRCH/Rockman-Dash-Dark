#include "GameObject/UI_FadeIn.h"

#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CUI_FadeIn::CUI_FadeIn()
{
	Set_Name(TEXT("UI_FadeIn"));
}

CUI_FadeIn::CUI_FadeIn(const CUI_FadeIn& rhs)
	: Base(rhs)
{
}

HRESULT CUI_FadeIn::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

	return S_OK;
}

HRESULT CUI_FadeIn::Initialize(void* Arg)
{
	return S_OK;
}

void CUI_FadeIn::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CUI_FadeIn::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	m_State_Act.Get_StateFunc()(this, fTimeDelta);
}

void CUI_FadeIn::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_FadeIn::Render()
{
	SUPER::Render();

	auto pEffect = m_pPlaneModelComp->EffectComp();
	if (pEffect)
	{
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(0);
		m_pPlaneModelComp->BindAndRender_Buffer(1);
	}

	return S_OK;
}

void CUI_FadeIn::OnCreated()
{
	SUPER::OnCreated();

	Register_State();
	Set_RenderGroup(ERenderGroup::UI);
	Set_Priority(0, -1.f);
}

void CUI_FadeIn::BeginPlay()
{
	SUPER::BeginPlay();

	Transform().Set_Scale((_float)g_iWindowSizeX, (_float)g_iWindowSizeX, 1.f);
	//m_fUVSize.Increase(1.f);
}

CUI_FadeIn* CUI_FadeIn::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_FadeIn Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_FadeIn::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CUI_FadeIn Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CUI_FadeIn::Free()
{
	SUPER::Free();
}

FSerialData CUI_FadeIn::SerializeData()
{
	return FSerialData();
}

HRESULT CUI_FadeIn::Initialize_Component()
{
	// ������Ÿ�Կ� ������Ʈ �ʱ�ȭ
	if (FAILED(Add_Component(L"PlaneComp", m_pPlaneModelComp = CPlaneModelComp::Create())))
		return E_FAIL;

	m_pPlaneModelComp->Set_Mode(CPlaneModelComp::ORTHO);
	m_pPlaneModelComp->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/FadeOut/BlackScreen.png"), 1);
	m_pPlaneModelComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);

	return S_OK;
}

HRESULT CUI_FadeIn::Bind_ShaderResources()
{
	if (m_pPlaneModelComp)
	{
		auto pEffect = m_pPlaneModelComp->EffectComp();
		if (pEffect)
		{
			_float4x4 TempFloat4x4 = Transform().Get_TransformFloat4x4();
			_float TempFloat = 1.f - m_fAlpha.fCur;
			_float2 TempFloat2 = {};

			if (FAILED(Transform().Bind_TransformToEffect(pEffect, "g_WorldMatrix")))
				return E_FAIL;
			if (FAILED(pEffect->Bind_Matrix("g_ViewMatrix", &(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Ortho, ECamMatrix::View, ECamNum::One)))))
				return E_FAIL;
			if (FAILED(pEffect->Bind_Matrix("g_ProjMatrix", &(TempFloat4x4 = PipelineComp().Get_CamFloat4x4(ECamType::Ortho, ECamMatrix::Proj, ECamNum::One)))))
				return E_FAIL;
			if (FAILED(pEffect->Bind_RawValue("g_fAlpha", &TempFloat, sizeof(_float))))
				return E_FAIL;
			if (FAILED(pEffect->Bind_RawValue("g_vMinUV", &(TempFloat2 = { 0.f, 0.f }), sizeof(_float2))))
				return E_FAIL;
			if (FAILED(pEffect->Bind_RawValue("g_vMaxUV", &(TempFloat2 = { 1.f, 1.f }), sizeof(_float2))))
				return E_FAIL;
		}
	}

	return S_OK;
}

void CUI_FadeIn::Register_State()
{
	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::FadeOut, &ThisClass::ActState_FadeOut);
	m_State_Act.Set_State(EState_Act::Idle);
}

void CUI_FadeIn::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{

	}

	if (m_State_Act.Can_Update())
	{

		//if (!m_EndEvent.empty())
		m_State_Act.Set_State(EState_Act::FadeOut);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CUI_FadeIn::ActState_FadeOut(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{

	}

	if (m_State_Act.Can_Update())
	{
		if (m_fAlpha.Increase(0.7f * fTimeDelta))
		{
			if (!m_EndEvent.empty())
			{
				m_EndEvent();
				Set_Dead();
			}
		}
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CUI_FadeIn::Add_Event(StartDelegate StartEvent, EndDelegate EndEvent)
{
	m_StartEvent = StartEvent;
	m_EndEvent = EndEvent;
}
