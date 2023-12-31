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

HRESULT CGameObject::Initialize(void* Arg)
{
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

void CGameObject::Free()
{
	for (auto iter = m_vecComponent.begin(); iter != m_vecComponent.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	m_vecComponent.clear();

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

	auto iter = find_if(m_vecComponent.begin(), m_vecComponent.end(), 
		[&strName](CGameObjectComp* pComp) {
			return pComp->Get_Name() == strName;
		});

	// �̸� �ߺ��� �߰����� ����, ���� �̸��������� �߰��ϵ��� ����
	if (iter != m_vecComponent.end())
	{
		Safe_Release(pComponent);
		return E_FAIL;
	}

	// �Ϸ�� ���Ϳ� ������Ʈ �߰�
	m_vecComponent.push_back(pComponent);
	pComponent->Set_OwnerObject(this);
	pComponent->Set_Name(strName);

	pComponent->Set_StateUpdate_Event<ThisClass>(this, &ThisClass::OnStateUpdate_Updated);
	pComponent->Set_StateLateUpdate_Event<ThisClass>(this, &ThisClass::OnStateLateUpdate_Updated);
	pComponent->Set_StateRender_Event<ThisClass>(this, &ThisClass::OnStateRender_Updated);

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

