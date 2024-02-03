#include "GameObject/ItemChest.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "Utility/RapidJsonSerial.h"
#include "GameObject/Player.h"
#include "GameObject/UI_Dialog.h"

CItemChest::CItemChest()
{
	Set_Name(TEXT("ItemChest"));
}

CItemChest::CItemChest(const CItemChest& rhs)
{
	
}

HRESULT CItemChest::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Chest/Chest.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Position(0.f, 1.f, 0.f);
	m_pColliderComp->Transform().Set_Scale(1.0f, 1.f, 1.0f);
	m_pColliderComp->Bind_Collision(ECollisionType::OBB);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_OBJECT);

	TeamAgentComp().Set_TeamID(ETEAM_NUTRAL);

	return S_OK;
}

HRESULT CItemChest::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	_uint iNumPrototype = 0;
	iNumPrototype = InputData.Get_ArraySize("Components");
	for (_uint i = 0; i < iNumPrototype; i++)
	{
		FSerialData ProtoData;
		InputData.Get_ObjectFromArray("Components", i, ProtoData);

		_uint iComponentID = 0;
		if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
			return E_FAIL;

		string strName = "";
		if (FAILED(ProtoData.Get_Data("Name", strName)))
			return E_FAIL;

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			NULL_CHECK_RETURN(m_pModelComp = CCommonModelComp::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(ConvertToWstring(strName), m_pModelComp)))
				return E_FAIL;
			m_pModelComp->Set_Animation(0, 1.f, true);
			break;
		case ECast(EComponentID::Collider):
			NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(ConvertToWstring(strName), m_pColliderComp)))
				return E_FAIL;
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_NUTRAL);

	return S_OK;
}

HRESULT CItemChest::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CItemChest::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

	return S_OK;
}

void CItemChest::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CItemChest::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	m_ActionKey.Reset();
	m_State_Act.Get_StateFunc()(this, fTimeDelta);
}

void CItemChest::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);

	if (IsState(EGObjectState::Drawing))
	{
		m_pModelComp->Invalidate_Animation();
		m_pModelComp->Late_Tick(fTimeDelta);
	}
}

HRESULT CItemChest::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

void CItemChest::OnCreated()
{
	SUPER::OnCreated();

	Register_State();
}

void CItemChest::BeginPlay()
{
	SUPER::BeginPlay();

	if (m_pColliderComp)
		m_pColliderComp->EnterToPhysics(0);
}

CItemChest* CItemChest::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CItemChest Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CItemChest* CItemChest::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CItemChest Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemChest::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CItemChest Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CItemChest::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CItemChest Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CItemChest::Free()
{
	SUPER::Free();
}

HRESULT CItemChest::Initialize_Component()
{
	return S_OK;
}

HRESULT CItemChest::Initialize_Component(FSerialData& InputData)
{
	_uint iNumPrototype = 0;
	iNumPrototype = InputData.Get_ArraySize("Components");
	for (_uint i = 0; i < iNumPrototype; i++)
	{
		FSerialData InputProto;
		InputData.Get_ObjectFromArray("Components", i, InputProto);

		_uint iComponentID = 0;
		if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
			return E_FAIL;

		string strProtoName = "";
		if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
			return E_FAIL;

		string strName = "";
		if (FAILED(InputProto.Get_Data("Name", strName)))
			return E_FAIL;

		switch (iComponentID)
		{
		case ECast(EComponentID::CommonModel):
			FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
				m_pModelComp = DynCast<CCommonModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
			m_pModelComp->Set_Animation(0, 1.f, true);
			break;
		case ECast(EComponentID::Collider):
			FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
				m_pColliderComp = DynCast<CColliderComponent*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			break;
		}
	}

	TeamAgentComp().Set_TeamID(ETEAM_NUTRAL);

	return S_OK;
}

FSerialData CItemChest::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	Data.Add_Member("ClassID", g_ClassID);

	return Data;
}

FSerialData CItemChest::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);

	return Data;
}

void CItemChest::Register_State()
{
	for (_uint i = 0; i < ECast(EActionKey::Size); i++)
		m_ActionKey.Add_Action(Cast<EActionKey>(i));

	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::Open, &ThisClass::ActState_Open);
	m_State_Act.Add_Func(EState_Act::Close, &ThisClass::ActState_Close);
	m_State_Act.Set_State(EState_Act::Idle);
}

void CItemChest::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(0, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{
		// 열림
		if (m_ActionKey.IsAct(EActionKey::Open))
			m_State_Act.Set_State(EState_Act::Open);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CItemChest::Open_Chest(GiveItemDelegate Event, EndDelegate EndEvent)
{
	if (m_State_Act.IsOnState(EState_Act::Idle))
	{
		m_State_Act.Set_State(EState_Act::Open);
		// 대충 아이템 주는 이벤트 발생
		
		m_GiveItemEvent = Event;
		m_EndEvent = EndEvent;

		CUI_Dialog* pDialog = CUI_Dialog::Create();
		pDialog->Add_Dialog(TEXT("안에 무언가가 들어있다..."));
		pDialog->Add_Dialog(TEXT("여러가지 무기들을 얻었다!"), MakeDelegate(this, &ThisClass::Give_Item));
		pDialog->Set_EndEvent(MakeDelegate(this, &ThisClass::Close_Chest));
		GI()->Add_GameObject(pDialog);
	}
	else if (m_State_Act.IsOnState(EState_Act::Close))
	{
		m_GiveItemEvent = Event;
		m_EndEvent = EndEvent;

		CUI_Dialog* pDialog = CUI_Dialog::Create();
		pDialog->Add_Dialog(TEXT("안에는 아무것도 없다..."));
		pDialog->Set_EndEvent(MakeDelegate(this, &ThisClass::NothingInChest));
		GI()->Add_GameObject(pDialog);
	}
}

void CItemChest::Close_Chest()
{
	m_State_Act.Set_State(EState_Act::Close);
}

void CItemChest::Give_Item()
{
	if (!m_GiveItemEvent.empty())
	{
		m_GiveItemEvent(true, EItemObtain::SubWeapons);
		GI()->Play_Sound(TEXT("RockmanDash2"), TEXT("gotcha.mp3"), CHANNELID::SOUND_VFX, 1.f);
	}
	m_GiveItemEvent.clear();
}

void CItemChest::NothingInChest()
{
	if (!m_EndEvent.empty())
	{
		m_EndEvent();
		m_EndEvent.clear();
	}
	m_GiveItemEvent.clear();
}

void CItemChest::ActState_Open(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(2, 1.f, false, true);
		m_fItemGiveDelay.Reset();
	}

	if (m_State_Act.Can_Update())
	{
		// 닫힘
		if (m_ActionKey.IsAct(EActionKey::Close))
			m_State_Act.Set_State(EState_Act::Close);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CItemChest::ActState_Close(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(2, 1.f, false, false);
		if (!m_EndEvent.empty())
		{
			m_EndEvent();
			m_EndEvent.clear();
		}
	}

	if (m_State_Act.Can_Update())
	{

	}

	if (m_State_Act.IsState_Exit())
	{

	}
}