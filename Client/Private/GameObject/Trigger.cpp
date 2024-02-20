#include "GameObject/Trigger.h"

#include "Component/CommonModelComp.h"
#include "Component/ColliderComponent.h"

#include "GameObject/Player.h"
#include "Utility/ClassID.h"
#include "GameObject/UI_Boss.h"



CTrigger::CTrigger()
{
	Set_Name(TEXT("Trigger"));
}

CTrigger::CTrigger(const CTrigger& rhs)
	: Base(rhs)
{

}

HRESULT CTrigger::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	FAILED_CHECK_RETURN(Add_Component(L"ColliderComp", m_pColliderComp = CColliderComponent::Create()), E_FAIL);
	m_pColliderComp->Transform().Set_Position(0.f, 0.f, 0.f);
	m_pColliderComp->Transform().Set_Scale(1.f, 1.f, 1.f);
	m_pColliderComp->Bind_Collision(ECollisionType::OBB);
	m_pColliderComp->Set_CollisionLayer(COLLAYER_OBJECT);
	m_pColliderComp->Set_CollisionMask(COLLAYER_CHARACTER);

	return S_OK;
}

HRESULT CTrigger::Initialize_Prototype(FSerialData& InputData)
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
		case ECast(EComponentID::Collider):
			NULL_CHECK_RETURN(m_pColliderComp = CColliderComponent::Create(ProtoData), E_FAIL);
			if (FAILED(Add_Component(ConvertToWstring(strName), m_pColliderComp)))
				return E_FAIL;
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			m_pColliderComp->Set_CollisionKinematic();
			break;
		}
	}

	_uint iType = { 0 };
	InputData.Get_Data("Type", iType);
	m_eTriggerType = Cast<EType>(iType);

	return S_OK;
}

HRESULT CTrigger::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTrigger::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

	return S_OK;
}

void CTrigger::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CTrigger::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	switch (m_eTriggerType)
	{
	case EType::Sound:
		m_pGI->Play_BGM_LoopRange(TEXT("RockmanDash2"), TEXT("44. Calbania - Kito Caverns.mp3"), 1.f, 0U, 65'454U);
		Set_Dead();
		break;
	}

	m_pColliderComp->Tick(fTimeDelta);
}

void CTrigger::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CTrigger::Render()
{
	SUPER::Render();


#ifdef _DEBUG
	m_pGI->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

void CTrigger::BeginPlay()
{
	SUPER::BeginPlay();

	if (m_pColliderComp)
		m_pColliderComp->EnterToPhysics(0);
}

CTrigger* CTrigger::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Trigger Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CTrigger* CTrigger::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("Trigger Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrigger::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Trigger Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CTrigger::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("Trigger Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CTrigger::Free()
{
	SUPER::Free();
}

FSerialData CTrigger::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("Type", ECast(m_eTriggerType));

	return Data;
}

FSerialData CTrigger::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ClassID", g_ClassID);
	Data.Add_Member("Type", ECast(m_eTriggerType));

	return Data;
}

HRESULT CTrigger::Initialize_Component()
{
	return S_OK;
}

HRESULT CTrigger::Initialize_Component(FSerialData& InputData)
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
		case ECast(EComponentID::Collider):
			FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
				m_pColliderComp = DynCast<CColliderComponent*>(m_pGI->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
			m_pColliderComp->Set_Collision_Event(MakeDelegate(this, &ThisClass::OnCollision));
			m_pColliderComp->Set_CollisionEntered_Event(MakeDelegate(this, &ThisClass::OnCollisionEntered));
			m_pColliderComp->Set_CollisionExited_Event(MakeDelegate(this, &ThisClass::OnCollisionExited));
			m_pColliderComp->Set_CollisionKinematic();
			break;
		}
	}

	_uint iType = { 0 };
	InputData.Get_Data("Type", iType);
	m_eTriggerType = Cast<EType>(iType);

	return S_OK;
}

void CTrigger::OnCollision(CGameObject* pDst, const FContact* pContact)
{

}

void CTrigger::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{

}

void CTrigger::OnCollisionExited(CGameObject* pDst)
{
	CPlayer* pPlayer = DynCast<CPlayer*>(pDst);
	if (pPlayer)
	{
		switch (m_eTriggerType)
		{
		case EType::Boss:
			m_pGI->Add_GameObject(CUI_Boss::Create());
			m_pGI->Play_BGM(TEXT("RockmanDash2"), TEXT("13. Forbidden Island - Rush Mammoo.mp3"), 1.f);
			break;
		default:
			break;
		}

		Set_Dead();
	}
}
