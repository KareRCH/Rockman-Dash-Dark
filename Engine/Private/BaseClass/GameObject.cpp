#include "BaseClass/GameObject.h"


#include "System/GameInstance.h"

CGameObject::CGameObject()
	: m_pPipelineComp(Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp")))
{
	m_pTransformComp = CTransformComponent::Create();

	TurnOn_State(EGObjectState::Priority_Tick);
	TurnOn_State(EGObjectState::Tick);
	TurnOn_State(EGObjectState::Late_Tick);
	TurnOn_State(EGObjectState::Render);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pPipelineComp(rhs.m_pPipelineComp)
	, m_iStateFlag(rhs.m_iStateFlag)
	, m_strPrototypeName(rhs.m_strPrototypeName)
{
	// ���������� ����
	Safe_AddRef(m_pPipelineComp);

	for (_uint i = 0; i < ECast(EGObjTickPriority::Size); i++)
		m_fPriority[i] = rhs.m_fPriority[i];

	for (_uint i = 0; i < ECast(EGObjTag::Size); i++)
		m_setTag[i] = rhs.m_setTag[i];

	// �����ϴ� ��쵵 �־ �ϴ��� �����Ѵ�.
	m_pTransformComp = Cast<CTransformComponent*>(rhs.m_pTransformComp->Clone());
}

HRESULT CGameObject::Initialize_Prototype()
{
	// �⺻ Ʈ������ ����
	// �Ŵ��� ���� ������Ʈ�� �ܼ� ���۷��������� ���� ������Ʈ�� ���ȴ�.
	// ī�޶� ��Ŀ� ���� ������ ��ȯ�� �ϴ� ������Ʈ�̴�. ���������ο� GI���ؼ� �������� �ʰ�
	// �ش� �Ŵ����� ���� ������Ʈ�� ����Ѵ�.

	return S_OK;
}

HRESULT CGameObject::Initialize_Prototype(FSerialData& InputData)
{
	string strName = "";
	if (FAILED(InputData.Get_Data("Name", strName)))
		return E_FAIL;
	m_strName = ConvertToWstring(strName);

	string strProtoName = "";
	if (FAILED(InputData.Get_Data("ProtoName", strProtoName)))
		return E_FAIL;
	m_strPrototypeName = ConvertToWstring(strProtoName);

	if (FAILED(InputData.Get_Data("PriorityTick", m_fPriority[ECast(EGObjTickPriority::Tick)])))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("PriorityRender", m_fPriority[ECast(EGObjTickPriority::Render)])))
		return E_FAIL;

	_float3 vPos = {}, vRot = {}, vScale = {};
	if (FAILED(InputData.Get_Data("PosX", vPos.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("PosY", vPos.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("PosZ", vPos.z)))
		return E_FAIL;

	Transform().Set_Position(vPos);

	if (FAILED(InputData.Get_Data("RotX", vRot.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("RotY", vRot.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("RotZ", vRot.z)))
		return E_FAIL;

	vRot.x = XMConvertToRadians(vRot.x);
	vRot.y = XMConvertToRadians(vRot.y);
	vRot.z = XMConvertToRadians(vRot.z);
	Transform().Set_RotationFixed(vRot);

	if (FAILED(InputData.Get_Data("ScaleX", vScale.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("ScaleY", vScale.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("ScaleZ", vScale.z)))
		return E_FAIL;

	Transform().Set_Scale(vScale);

	return S_OK;
}

HRESULT CGameObject::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CGameObject::Initialize(FSerialData& InputData)
{
	_float3 vPos = {}, vRot = {}, vScale = {};
	if (FAILED(InputData.Get_Data("PosX", vPos.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("PosY", vPos.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("PosZ", vPos.z)))
		return E_FAIL;

	Transform().Set_Position(vPos);

	if (FAILED(InputData.Get_Data("RotX", vRot.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("RotY", vRot.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("RotZ", vRot.z)))
		return E_FAIL;

	vRot.x = XMConvertToRadians(vRot.x);
	vRot.y = XMConvertToRadians(vRot.y);
	vRot.z = XMConvertToRadians(vRot.z);
	Transform().Set_RotationFixed(vRot);

	if (FAILED(InputData.Get_Data("ScaleX", vScale.x)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("ScaleY", vScale.y)))
		return E_FAIL;
	if (FAILED(InputData.Get_Data("ScaleZ", vScale.z)))
		return E_FAIL;

	Transform().Set_Scale(vScale);

	return S_OK;
}

void CGameObject::Priority_Tick(const _float& fTimeDelta)
{
	_uint iIndex = ECast(ECompTickType::Priority);

	if (m_listUpdateComp[iIndex].empty())
		return;

	for (auto iter = m_listUpdateComp[iIndex].begin(); iter != m_listUpdateComp[iIndex].end(); ++iter)
	{
		(*iter)->Tick(fTimeDelta);
	}
}

void CGameObject::Tick(const _float& fTimeDelta)
{
	_uint iIndex = ECast(ECompTickType::Tick);

	for (auto iter = m_listUpdateComp[iIndex].begin(); iter != m_listUpdateComp[iIndex].end(); ++iter)
	{
		(*iter)->Tick(fTimeDelta);
	}
}

void CGameObject::Late_Tick(const _float& fTimeDelta)
{
	_uint iIndex = ECast(ECompTickType::Late);

	for (auto iter = m_listUpdateComp[iIndex].begin(); iter != m_listUpdateComp[iIndex].end(); ++iter)
	{
		(*iter)->Late_Tick(fTimeDelta);
	}
}

HRESULT CGameObject::Render()
{
	for (auto iter = m_listUpdateComp[ECast(ECompTickType::Render)].begin();
		iter != m_listUpdateComp[ECast(ECompTickType::Render)].end(); ++iter)
	{
		(*iter)->Render();
	}

	return S_OK;
}

FSerialData CGameObject::SerializeData_Prototype()
{
	FSerialData Data;

	Data.Add_MemberString("Name", ConvertToString(m_strName));
	Data.Add_MemberString("ProtoName", ConvertToString(m_strPrototypeName));
	Data.Add_Member("ClassID", 0U);		// �⺻��
	Data.Add_Member("StateFlag", m_iStateFlag);

	Data.Add_Member("PriorityTick", m_fPriority[ECast(EGObjTickPriority::Tick)]);
	Data.Add_Member("PriorityRender", m_fPriority[ECast(EGObjTickPriority::Render)]);

	_float3 vPos = Transform().Get_PositionFloat3();
	Data.Add_Member("PosX", vPos.x);
	Data.Add_Member("PosY", vPos.y);
	Data.Add_Member("PosZ", vPos.z);

	_float3 vRot = Transform().Get_RotationFixedFloat3();
	Data.Add_Member("RotX", XMConvertToDegrees(vRot.x));
	Data.Add_Member("RotY", XMConvertToDegrees(vRot.y));
	Data.Add_Member("RotZ", XMConvertToDegrees(vRot.z));

	_float3 vScale = Transform().Get_ScaleFloat3();
	Data.Add_Member("ScaleX", vScale.x);
	Data.Add_Member("ScaleY", vScale.y);
	Data.Add_Member("ScaleZ", vScale.z);

	for (auto& pObj : m_vecComponents)
	{
		FSerialData CompData = pObj->SerializeData_Prototype();
		Data.Pushback_Member("Components", CompData);
	}

	return Data;
}

FSerialData CGameObject::SerializeData()
{
	FSerialData Data;

	Data.Add_MemberString("Name", ConvertToString(m_strName));
	Data.Add_MemberString("ProtoName", ConvertToString(m_strPrototypeName));
	Data.Add_Member("ClassID", 0U);		// �⺻��
	Data.Add_Member("StateFlag", m_iStateFlag);

	Data.Add_Member("PriorityTick", m_fPriority[ECast(EGObjTickPriority::Tick)]);
	Data.Add_Member("PriorityRender", m_fPriority[ECast(EGObjTickPriority::Render)]);

	_float3 vPos = Transform().Get_PositionFloat3();
	Data.Add_Member("PosX", vPos.x);
	Data.Add_Member("PosY", vPos.y);
	Data.Add_Member("PosZ", vPos.z);

	_float3 vRot = Transform().Get_RotationFixedFloat3();
	Data.Add_Member("RotX", XMConvertToDegrees(vRot.x));
	Data.Add_Member("RotY", XMConvertToDegrees(vRot.y));
	Data.Add_Member("RotZ", XMConvertToDegrees(vRot.z));

	_float3 vScale = Transform().Get_ScaleFloat3();
	Data.Add_Member("ScaleX", vScale.x);
	Data.Add_Member("ScaleY", vScale.y);
	Data.Add_Member("ScaleZ", vScale.z);

	for (auto& pObj : m_vecComponents)
	{
		FSerialData CompData = pObj->SerializeData();
		Data.Pushback_Member("Components", CompData);
	}

	return Data;
}

void CGameObject::Free()
{
	for (auto iter = m_vecComponents.begin(); iter != m_vecComponents.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	m_vecComponents.clear();

	Release_Transform();
	Safe_Release(m_pPipelineComp);
}

void CGameObject::Delete_Tag(const EGObjTag eTagType, const wstring& strTag)
{
	auto iter = m_setTag[ECast(eTagType)].find(strTag);

	if (iter != m_setTag[ECast(eTagType)].end())
		m_setTag[ECast(eTagType)].erase(iter);
}




//------------------------------------------------



HRESULT CGameObject::Add_Component(const wstring& strName, CGameObjectComp* pComponent)
{
	if (nullptr == pComponent)
		return E_FAIL;

	auto iter = find_if(m_vecComponents.begin(), m_vecComponents.end(), 
		[&strName](CGameObjectComp* pComp) {
			return pComp->Get_Name() == strName;
		});

	// �̸� �ߺ��� �߰����� ����, ���� �̸��������� �߰��ϵ��� ����
	if (iter != m_vecComponents.end())
	{
		Safe_Release(pComponent);
		return E_FAIL;
	}

	// �Ϸ�� ���Ϳ� ������Ʈ �߰�
	m_vecComponents.push_back(pComponent);
	pComponent->Set_OwnerObject(this);
	pComponent->Set_Name(strName);

	pComponent->Set_StateUpdate_Event<ThisClass>(this, &ThisClass::OnStateUpdate_Updated);
	pComponent->Set_StateLateUpdate_Event<ThisClass>(this, &ThisClass::OnStateLateUpdate_Updated);
	pComponent->Set_StateRender_Event<ThisClass>(this, &ThisClass::OnStateRender_Updated);

	++m_iNumComponents;

	return S_OK;
}





//------------------------------------------------







void CGameObject::OnStateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue)
{
	_uint iIndex = ECast(ECompTickType::Tick);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(), m_listUpdateComp[iIndex].end(),
		[&pComp](CGameObjectComp* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[iIndex].end())
	{
		if (bValue == ECompTickAuto::Manual)
			m_listUpdateComp[iIndex].erase(iter);
	}
	else
	{
		if (bValue == ECompTickAuto::SemiAuto)
			m_listUpdateComp[iIndex].push_back(const_cast<CGameObjectComp*>(pComp));
	}
}

void CGameObject::OnStateLateUpdate_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue)
{
	_uint iIndex = ECast(ECompTickType::Late);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(),
		m_listUpdateComp[iIndex].end(),
		[&pComp](CGameObjectComp* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[iIndex].end())
	{
		if (bValue == ECompTickAuto::Manual)
			m_listUpdateComp[iIndex].erase(iter);
	}
	else
	{
		if (bValue == ECompTickAuto::SemiAuto)
			m_listUpdateComp[iIndex].push_back(const_cast<CGameObjectComp*>(pComp));
	}
}

void CGameObject::OnStateRender_Updated(const CGameObjectComp* const pComp, const ECompTickAuto& bValue)
{
	constexpr _uint iIndex = ECast(ECompTickType::Render);

	auto iter = find_if(m_listUpdateComp[0].begin(),
		m_listUpdateComp[0].end(),
		[&pComp](CGameObjectComp* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[0].end())
	{
		if (bValue == ECompTickAuto::Manual)
			m_listUpdateComp[0].erase(iter);
	}
	else
	{
		if (bValue == ECompTickAuto::SemiAuto)
			m_listUpdateComp[0].push_back(const_cast<CGameObjectComp*>(pComp));
	}
}

