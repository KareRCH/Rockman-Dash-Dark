#include "BaseClass/GameObject.h"

#include "Component/PrimitiveComponent.h"


CGameObject::CGameObject(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

CGameObject::CGameObject(const CGameObject& rhs)
	: m_pDevice(rhs.m_pDevice), m_pDeviceContext(rhs.m_pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CGameObject::Initialize()
{
	FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

	return S_OK;
}

void CGameObject::Priority_Tick(const _float& fTimeDelta)
{
}

_int CGameObject::Tick(const _float& fTimeDelta)
{
	for (auto iter = m_listUpdateComp[Cast_Uint(EUPDATE_T::UPDATE)].begin();
		iter != m_listUpdateComp[Cast_Uint(EUPDATE_T::UPDATE)].end(); ++iter)
	{
		(*iter)->Tick(fTimeDelta);
	}

	return 0;
}

void CGameObject::Late_Tick(const _float& fTimeDelta)
{
	for (auto iter = m_listUpdateComp[Cast_Uint(EUPDATE_T::LATE)].begin(); 
		iter != m_listUpdateComp[Cast_Uint(EUPDATE_T::LATE)].end(); ++iter)
	{
		(*iter)->Late_Tick(fTimeDelta);
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
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

	for (auto iter = m_vecComponent.begin(); iter != m_vecComponent.end(); ++iter)
	{
		Safe_Release((*iter));
	}
	m_vecComponent.clear();
}

void CGameObject::Delete_Tag(const wstring& strTag)
{
	auto iter = m_setTag.find(strTag);

	if (iter != m_setTag.end())
		m_setTag.erase(iter);
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
	constexpr _uint iIndex = Cast_EnumDef(EUPDATE_T::RENDER);

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
	FAILED_CHECK_RETURN(Add_Component(L"Transform", m_pTransformComp = CTransformComponent::Create({ m_pDevice, m_pDeviceContext })), E_FAIL);

	return S_OK;
}
