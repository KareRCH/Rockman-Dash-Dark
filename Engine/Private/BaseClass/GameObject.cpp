#include "BaseClass/GameObject.h"

#include "Component/PrimitiveComponent.h"


CGameObject::CGameObject()
{
}

CGameObject::CGameObject(const CGameObject& rhs)
{
}

HRESULT CGameObject::Initialize()
{
	FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

	return S_OK;
}

_int CGameObject::Tick(const _float& fTimeDelta)
{
	for (auto iter = m_listUpdateComp[Cast_Uint(EUPDATE_T::UPDATE)].begin();
		iter != m_listUpdateComp[Cast_Uint(EUPDATE_T::UPDATE)].end(); ++iter)
	{
		(*iter)->Update(fTimeDelta);
	}

	return 0;
}

void CGameObject::LateTick()
{
	for (auto iter = m_listUpdateComp[Cast_Uint(EUPDATE_T::LATE)].begin(); 
		iter != m_listUpdateComp[Cast_Uint(EUPDATE_T::LATE)].end(); ++iter)
	{
		(*iter)->LateUpdate();
	}
}

void CGameObject::Render()
{
	for (auto iter = m_listUpdateComp[Cast_Uint(EUPDATE_T::RENDER)].begin();
		iter != m_listUpdateComp[Cast_Uint(EUPDATE_T::RENDER)].end(); ++iter)
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
}




//------------------------------------------------



void CGameObject::Add_Component(const wstring& strName, CPrimitiveComponent* pComponent)
{
	auto iter = find_if(m_vecComponent.begin(), m_vecComponent.end(), 
		[&strName](CPrimitiveComponent* pComp) {
			return pComp->Get_Name() == strName;
		});

	// 이름 중복시 추가하지 않음, 추후 이름변경으로 추가하도록 변경
	if (iter != m_vecComponent.end())
	{
		Safe_Release(pComponent);
		return;
	}

	pComponent->Set_StateUpdate_Event<ThisClass>(this, &ThisClass::OnStateUpdate_Updated);
	pComponent->Set_StateLateUpdate_Event<ThisClass>(this, &ThisClass::OnStateLateUpdate_Updated);
	pComponent->Set_StateRender_Event<ThisClass>(this, &ThisClass::OnStateRender_Updated);
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







void CGameObject::OnStateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue)
{
	_uint iIndex = Cast_EnumDef(EUPDATE_T::UPDATE);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(), m_listUpdateComp[iIndex].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[iIndex].end())
	{
		if (bValue == ECOMP_UPDATE_TYPE::MANUAL)
			m_listUpdateComp[iIndex].erase(iter);
	}
	else
	{
		if (bValue == ECOMP_UPDATE_TYPE::SEMI_AUTO)
			m_listUpdateComp[iIndex].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

void CGameObject::OnStateLateUpdate_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue)
{
	_uint iIndex = Cast_EnumDef(EUPDATE_T::LATE);

	auto iter = find_if(m_listUpdateComp[iIndex].begin(),
		m_listUpdateComp[iIndex].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[iIndex].end())
	{
		if (bValue == ECOMP_UPDATE_TYPE::MANUAL)
			m_listUpdateComp[iIndex].erase(iter);
	}
	else
	{
		if (bValue == ECOMP_UPDATE_TYPE::SEMI_AUTO)
			m_listUpdateComp[iIndex].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

void CGameObject::OnStateRender_Updated(const CPrimitiveComponent* const pComp, const ECOMP_UPDATE_T& bValue)
{
	_uint iIndex = Cast_EnumDef(EUPDATE_T::RENDER);

	auto iter = find_if(m_listUpdateComp[0].begin(),
		m_listUpdateComp[0].end(),
		[&pComp](CPrimitiveComponent* _pComp) {
			return pComp == _pComp;
		});

	if (iter != m_listUpdateComp[0].end())
	{
		if (bValue == ECOMP_UPDATE_TYPE::MANUAL)
			m_listUpdateComp[0].erase(iter);
	}
	else
	{
		if (bValue == ECOMP_UPDATE_TYPE::SEMI_AUTO)
			m_listUpdateComp[0].push_back(const_cast<CPrimitiveComponent*>(pComp));
	}
}

HRESULT CGameObject::Initialize_Component()
{
	//m_pTransformComp = new CTransformComponent();

	return S_OK;
}
