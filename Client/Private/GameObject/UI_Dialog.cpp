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

	//m_pPlaneModelComp->Render();
	if (!m_Dialogs.empty())
	{
		GI()->Render_Font(L"Default", m_Dialogs.front().substr(0, m_iCurDialogIndex), _float2(200.f, 200.f));
	}

	return S_OK;
}

void CUI_Dialog::OnCreated()
{
	SUPER::OnCreated();

	Register_State();
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
	m_pPlaneModelComp->Transform().Set_Scale(1.3f, 1.3f, 1.3f);
	m_pPlaneModelComp->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/UI/Dialog/Dialog-%d.png"), 9);
	m_pPlaneModelComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);

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

	}

	if (m_State_Act.Can_Update())
	{

		// 창이 다 켜지면 다음 상태로
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
		_uint iMaxIndex = Cast<_uint>(m_Dialogs.front().size());
		if (m_fDelay.Increase(fTimeDelta))
		{
			++m_iCurDialogIndex;
			if (m_iCurDialogIndex > iMaxIndex)
				m_iCurDialogIndex = iMaxIndex;
			else
				GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("dialog_process.mp3"), CHANNELID::SOUND_UI, 1.f);
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

	}

	if (m_State_Act.Can_Update())
	{

		// 모두 닫히면 종료
		Set_Dead();
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}
