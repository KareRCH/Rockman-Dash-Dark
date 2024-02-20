#include "GameObject/Npc_Man1.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "Utility/RapidJsonSerial.h"
#include "GameObject/Player.h"
#include "GameObject/UI_Dialog.h"

CNpc_Man1::CNpc_Man1()
{
	Set_Name(TEXT("Npc_Man1"));
}

CNpc_Man1::CNpc_Man1(const CNpc_Man1& rhs)
{

}

HRESULT CNpc_Man1::Initialize_Prototype()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_Scale(_float3(0.5f, 0.5f, 0.5f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelAnim.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Chest/Chest.amodel");

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Position(0.f, 1.f, 0.f);
	m_pColliderComp->Transform().Set_Scale(1.0f, 1.f, 1.0f);
	m_pColliderComp->Bind_Collision(ECollisionType::Capsule);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_CHARACTER);

	TeamAgentComp().Set_TeamID(ETEAM_NUTRAL);

	return S_OK;
}

HRESULT CNpc_Man1::Initialize_Prototype(FSerialData& InputData)
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

HRESULT CNpc_Man1::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CNpc_Man1::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

	return S_OK;
}

void CNpc_Man1::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CNpc_Man1::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	m_ActionKey.Reset();
	m_State_Act.Get_StateFunc()(this, fTimeDelta);
}

void CNpc_Man1::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);

	if (IsState(EGObjectState::Drawing))
	{
		m_pModelComp->Invalidate_Animation();
		m_pModelComp->Late_Tick(fTimeDelta);
	}
}

HRESULT CNpc_Man1::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

void CNpc_Man1::OnCreated()
{
	SUPER::OnCreated();

	Register_State();
}

void CNpc_Man1::BeginPlay()
{
	SUPER::BeginPlay();

	if (m_pColliderComp)
		m_pColliderComp->EnterToPhysics(0);
}

CNpc_Man1* CNpc_Man1::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CNpc_Man1 Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CNpc_Man1* CNpc_Man1::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CNpc_Man1 Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNpc_Man1::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CNpc_Man1 Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CNpc_Man1::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CNpc_Man1 Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CNpc_Man1::Free()
{
	SUPER::Free();
}

HRESULT CNpc_Man1::Initialize_Component()
{
	return S_OK;
}

HRESULT CNpc_Man1::Initialize_Component(FSerialData& InputData)
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

FSerialData CNpc_Man1::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	Data.Add_Member("ClassID", g_ClassID);

	return Data;
}

FSerialData CNpc_Man1::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);

	return Data;
}

void CNpc_Man1::Register_State()
{
	for (_uint i = 0; i < ECast(EActionKey::Size); i++)
		m_ActionKey.Add_Action(Cast<EActionKey>(i));

	m_State_Act.Add_Func(EState_Act::Idle, &ThisClass::ActState_Idle);
	m_State_Act.Add_Func(EState_Act::Dialog, &ThisClass::ActState_Dialog);
	m_State_Act.Add_Func(EState_Act::EndDialog, &ThisClass::ActState_EndDialog);
	m_State_Act.Set_State(EState_Act::Idle);
}

void CNpc_Man1::ActState_Idle(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(0, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{

		if (m_ActionKey.IsAct(EActionKey::Dialog))
			m_State_Act.Set_State(EState_Act::Dialog);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CNpc_Man1::ActState_Dialog(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(0, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{


	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

void CNpc_Man1::ActState_EndDialog(const _float& fTimeDelta)
{
	if (m_State_Act.IsState_Entered())
	{
		m_pModelComp->Set_Animation(0, 1.f, false);
	}

	if (m_State_Act.Can_Update())
	{

		// 원래 바라보던 위치로 바라보면 Idle로
			m_State_Act.Set_State(EState_Act::Idle);
	}

	if (m_State_Act.IsState_Exit())
	{

	}
}

_bool CNpc_Man1::DialogStart(DialogStartDelegate Event, DialogEndDelegate EndEvent)
{
	if (m_State_Act.IsOnState(EState_Act::Idle))
	{
		m_DialogStartEvent = Event;
		m_DialogEndEvent = EndEvent;

		m_State_Act.Set_State(EState_Act::Dialog);

		return true;
	}
	
	return false;
}

void CNpc_Man1::DialogEnd()
{
	if (!m_DialogEndEvent.empty())
		m_DialogEndEvent();
}
