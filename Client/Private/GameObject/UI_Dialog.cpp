#include "GameObject/UI_Dialog.h"

#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CUI_Dialog::CUI_Dialog()
{
	Set_Name(TEXT("UI_Dialog"));
	Set_RenderGroup(ERenderGroup::UI);
}

CUI_Dialog::CUI_Dialog(const CUI_Dialog& rhs)
	: Base(rhs)
{
}

HRESULT CUI_Dialog::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

	return S_OK;
}

HRESULT CUI_Dialog::Initialize(void* Arg)
{
	return S_OK;
}

void CUI_Dialog::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CUI_Dialog::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	m_ActionKey.Reset();
	Input_Key();
	m_State_Act.Get_StateFunc()(this, fTimeDelta);

}

void CUI_Dialog::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_Dialog::Render()
{
	SUPER::Render();

	auto pEffect = m_pPlaneModelComp->EffectComp();
	if (pEffect)
	{
		Transform().Set_Position(XMLoadFloat3(&m_vPos[0]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[0]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(0);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[1]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[1]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(1);
		m_pPlaneModelComp->BindAndRender_Buffer(1);

		Transform().Set_Position(XMLoadFloat3(&m_vPos[2]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[2]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(2);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[3]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[3]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(3);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[4]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[4]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(4);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[5]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[5]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(5);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[6]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[6]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(6);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[7]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[7]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(7);
		m_pPlaneModelComp->BindAndRender_Buffer(1);


		Transform().Set_Position(XMLoadFloat3(&m_vPos[8]) + XMLoadFloat3(&m_vOffset));
		Transform().Set_Scale(m_vScale[8]);
		Bind_ShaderResources();
		m_pPlaneModelComp->Bind_Texture(8);
		m_pPlaneModelComp->BindAndRender_Buffer(1);
	}

	if (!m_Dialogs.empty())
	{
		_float2 vScreenSize = { Cast<_float>(GI()->ResolutionX()), Cast<_float>(GI()->ResolutionY()) };
		_float2 vTextPos = { vScreenSize.x * 0.5f, vScreenSize.y * 0.5f };
		vTextPos.x += -430.f + m_vOffset.x;
		vTextPos.y -= 100.f + m_vOffset.y;

		GI()->Render_Font(L"Default", m_Dialogs.front().first.substr(0, m_iCurDialogIndex), vTextPos);
	}

	return S_OK;
}

void CUI_Dialog::OnCreated()
{
	SUPER::OnCreated();

	Register_State();
	for (_uint i = 0; i < 9; i++)
	{
		m_vScale[i] = { 1.f, 1.f, 1.f };
	}
}

void CUI_Dialog::BeginPlay()
{
	SUPER::BeginPlay();
}

CUI_Dialog* CUI_Dialog::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Effect_Common Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Dialog::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Effect_Common Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CUI_Dialog::Free()
{
	SUPER::Free();
}

FSerialData CUI_Dialog::SerializeData()
{
	return FSerialData();
}

HRESULT CUI_Dialog::Initialize_Component()
{
	// 프로토타입용 컴포넌트 초기화
	if (FAILED(Add_Component(L"PlaneComp", m_pPlaneModelComp = CPlaneModelComp::Create())))
		return E_FAIL;

	m_pPlaneModelComp->Set_Mode(CPlaneModelComp::ORTHO);
	m_pPlaneModelComp->TextureComp()->Load_Texture(TEXT("Textures/RockmanDash2/UI/Dialog/Dialog-%d.png"), 9);
	m_pPlaneModelComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);

	return S_OK;
}

HRESULT CUI_Dialog::Bind_ShaderResources()
{
	if (m_pPlaneModelComp)
	{
		auto pEffect = m_pPlaneModelComp->EffectComp();
		if (pEffect)
		{
			_float4x4 TempFloat4x4 = Transform().Get_TransformFloat4x4();
			_float TempFloat = 1.f;
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

void CUI_Dialog::Register_State()
{
	for (_uint i = 0; i < ECast(EActionKey::Size); i++)
		m_ActionKey.Add_Action(Cast<EActionKey>(i));

	m_State_Act.Add_Func(EState_Act::Opening, &ThisClass::ActState_Opening);
	m_State_Act.Add_Func(EState_Act::Dialog, &ThisClass::ActState_Dialog);
	m_State_Act.Add_Func(EState_Act::Closing, &ThisClass::ActState_Closing);
	m_State_Act.Set_State(EState_Act::Opening);
}

void CUI_Dialog::Input_Key()
{
	if (GI()->IsKey_Pressed(DIK_SPACE))
		m_ActionKey.Act(EActionKey::Accept);
}

void CUI_Dialog::ActState_Opening(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_fLerp.Reset();
	}

	if (m_State_Act.Can_Update())
	{
		m_fLerp.Increase(10.f * fTimeDelta);

		_vector vSimPos[9] = {
			{ -450.f, 120.f, 1.f },
			{ 0.f, 120.f, 1.f },
			{ 450.f, 120.f, 1.f },
			{ -450.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 450.f, 0.f, 1.f },
			{ -450.f, -120.f, 1.f },
			{ 0.f, -120.f, 1.f },
			{ 450.f, -120.f, 1.f },
		};
		
		XMStoreFloat3(&m_vPos[0], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[0], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[1], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[1], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[2], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[2], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[3], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[3], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[4], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[4], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[5], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[5], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[6], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[6], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[7], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[7], m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[8], XMVectorLerp(XMVectorSet(0.f, 0.f, 1.f, 0.f), vSimPos[8], m_fLerp.fCur));

		_vector vSimScale[9] = {
			{ 10.f, 10.f, 1.f },
			{ 890.f, 10.f, 1.f },
			{ 10.f, 10.f, 1.f },
			{ 10.f, 230.f, 1.f },
			{ 890.f, 230.f, 1.f },
			{ 10.f, 230.f, 1.f },
			{ 10.f, 10.f, 1.f },
			{ 890.f, 10.f, 1.f },
			{ 10.f, 10.f, 1.f },
		};
		XMStoreFloat3(&m_vScale[0], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[0], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[1], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[1], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[2], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[2], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[3], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[3], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[4], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[4], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[5], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[5], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[6], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[6], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[7], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[7], m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[8], XMVectorLerp(XMVectorSet(1.f, 1.f, 1.f, 0.f), vSimScale[8], m_fLerp.fCur));

		// 창이 다 켜지면 다음 상태로
		if (m_fLerp.IsMax())
			m_State_Act.Set_State(EState_Act::Dialog);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CUI_Dialog::ActState_Dialog(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{

	}

	if (m_State_Act.Can_Update())
	{
		_uint iMaxIndex = Cast<_uint>(m_Dialogs.front().first.size());
		if (m_fDelay.Increase(fTimeDelta))
		{
			if (0 == m_iCurDialogIndex++)
			{
				auto& Event = m_Dialogs.front().second;
				if (!Event.empty())
				{
					Event();
					Event.clear();
				}
			}
			if (m_iCurDialogIndex > iMaxIndex)
				m_iCurDialogIndex = iMaxIndex;
			else
				GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("dialog_process.mp3"), CHANNELID::SOUND_UI1, 1.f);
			m_fDelay.Reset();
		}
		

		if (m_ActionKey.IsAct(EActionKey::Accept))
		{
			if (m_iCurDialogIndex >= iMaxIndex)
			{
				m_Dialogs.pop_front();
				m_iCurDialogIndex = 0;
			}
		}

		// 모든 대화가 끝나면 닫기
		if (m_Dialogs.empty())
			m_State_Act.Set_State(EState_Act::Closing);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CUI_Dialog::ActState_Closing(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_fLerp.Reset();
	}

	if (m_State_Act.Can_Update())
	{
		m_fLerp.Increase(10.f * fTimeDelta);

		_vector vSimPos[9] = {
			{ -450.f, 120.f, 1.f },
			{ 0.f, 120.f, 1.f },
			{ 450.f, 120.f, 1.f },
			{ -450.f, 0.f, 1.f },
			{ 0.f, 0.f, 1.f },
			{ 450.f, 0.f, 1.f },
			{ -450.f, -120.f, 1.f },
			{ 0.f, -120.f, 1.f },
			{ 450.f, -120.f, 1.f },
		};
		XMStoreFloat3(&m_vPos[0], XMVectorLerp(vSimPos[0], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[1], XMVectorLerp(vSimPos[1], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[2], XMVectorLerp(vSimPos[2], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[3], XMVectorLerp(vSimPos[3], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[4], XMVectorLerp(vSimPos[4], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[5], XMVectorLerp(vSimPos[5], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[6], XMVectorLerp(vSimPos[6], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[7], XMVectorLerp(vSimPos[7], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vPos[8], XMVectorLerp(vSimPos[8], XMVectorSet(0.f, 0.f, 1.f, 0.f), m_fLerp.fCur));

		_vector vSimScale[9] = {
			{ 10.f, 10.f, 1.f },
			{ 890.f, 10.f, 1.f },
			{ 10.f, 10.f, 1.f },
			{ 10.f, 230.f, 1.f },
			{ 890.f, 230.f, 1.f },
			{ 10.f, 230.f, 1.f },
			{ 10.f, 10.f, 1.f },
			{ 890.f, 10.f, 1.f },
			{ 10.f, 10.f, 1.f },
		};
		XMStoreFloat3(&m_vScale[0], XMVectorLerp(vSimScale[0], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[1], XMVectorLerp(vSimScale[1], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[2], XMVectorLerp(vSimScale[2], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[3], XMVectorLerp(vSimScale[3], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[4], XMVectorLerp(vSimScale[4], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[5], XMVectorLerp(vSimScale[5], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[6], XMVectorLerp(vSimScale[6], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[7], XMVectorLerp(vSimScale[7], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));
		XMStoreFloat3(&m_vScale[8], XMVectorLerp(vSimScale[8], XMVectorSet(1.f, 1.f, 1.f, 0.f), m_fLerp.fCur));

		// 모두 닫히면 종료
		if (m_fLerp.IsMax())
		{
			Set_Dead();
			// 끝 이벤트를 등록했다면 작동
			if (!m_EndEvent.empty())
				m_EndEvent();
		}
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CUI_Dialog::Add_Dialog(const wstring& strDialog, OccurDelegate Event)
{
	m_Dialogs.push_back({ strDialog, Event });
}
