#include "BaseClass/GameObject.h"

#include "Component/PrimitiveComponent.h"
#include "System/GameInstance.h"

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pPipelineComp(rhs.m_pPipelineComp)
{
	Safe_AddRef(m_pPipelineComp);
}

HRESULT CGameObject::Initialize_Prototype()
{
	m_pTransformComp = CTransformComponent::Create();
	m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp"));

	return S_OK;
}

HRESULT CGameObject::Initialize(void* Arg)
{
	m_pTransformComp = CTransformComponent::Create();

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

_int CGameObject::Tick(const _float& fTimeDelta)
{
	_uint iIndex = ECast(ECompTickType::Tick);

	for (auto iter = m_listUpdateComp[iIndex].begin(); iter != m_listUpdateComp[iIndex].end(); ++iter)
	{
		(*iter)->Tick(fTimeDelta);
	}

	return 0;
}

void CGameObject::Late_Tick(const _float& fTimeDelta)
{
	_uint iIndex = ECast(ECompTickType::Late);

	for (auto iter = m_listUpdateComp[iIndex].begin(); iter != m_listUpdateComp[iIndex].end(); ++iter)
	{
		(*iter)->Late_Tick(fTimeDelta);
	}
}

void CGameObject::Render()
{
	for (auto iter = m_listUpdateComp[ECast(ECompTickType::Render)].begin();
		iter != m_listUpdateComp[ECast(ECompTickType::Render)].end(); ++iter)
	{
		(*iter)->Render();
	}
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



HRESULT CGameObject::Add_Component(const wstring& strName, CPrimitiveComponent* pComponent)
{
	auto iter = find_if(m_vecComponent.begin(), m_vecComponent.end(), 
		[&strName](CPrimitiveComponent* pComp) {
			return pComp->Get_Name() == strName;
		});

	// 이름 중복시 추가하지 않음, 추후 이름변경으로 추가하도록 변경
	if (iter != m_vecComponent.end())
	{
		Safe_Release(pComponent);
		return E_FAIL;
	}

	// 완료시 벡터에 컴포넌트 추가
	m_vecComponent.push_back(pComponent);
	pComponent->Set_OwnerObject(this);

	pComponent->Set_StateUpdate_Event<ThisClass>(this, &ThisClass::OnStateUpdate_Updated);
	pComponent->Set_StateLateUpdate_Event<ThisClass>(this, &ThisClass::OnStateLateUpdate_Updated);
	pComponent->Set_StateRender_Event<ThisClass>(this, &ThisClass::OnStateRender_Updated);

	return S_OK;
}

CPrimitiveComponent* CGameObject::Get_Component(const wstring& strName)
{
	auto iter = find_if(m_vecComponent.begin(), m_vecComponent.end(),
		[&strName](CPrimitiveComponent* pComp) {
			return pComp->Get_Name() == strName;
		});

	if (iter != m_vecComponent.end())
	{
		return (*iter);
	}

	return nullptr;
}



//------------------------------------------------







void CGameObject::OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue)
{
	_uint iIndex = ECast(ECompTickType::Tick);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(), m_listUpdateComp[iIndex].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
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
			m_listUpdateComp[iIndex].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

void CGameObject::OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue)
{
	_uint iIndex = ECast(ECompTickType::Late);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(),
		m_listUpdateComp[iIndex].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
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
			m_listUpdateComp[iIndex].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

void CGameObject::OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECompTickAuto& bValue)
{
	constexpr _uint iIndex = ECast(ECompTickType::Render);

	auto iter = find_if(m_listUpdateComp[0].begin(),
		m_listUpdateComp[0].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
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
			m_listUpdateComp[0].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

