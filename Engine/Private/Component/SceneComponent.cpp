#include "Component/SceneComponent.h"

CSceneComponent::CSceneComponent(const DX11DEVICE_T tDeivce)
	: Base(tDeivce)
{
	
}

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
{
}

CSceneComponent::~CSceneComponent()
{
}

HRESULT CSceneComponent::Initialize()
{
	Add_InterComponent(m_pTransformComp->Get_Name(), m_pTransformComp = CTransformComponent::Create());
	Safe_AddRef(m_pTransformComp);

	return S_OK;
}

_int CSceneComponent::Tick(const _float& fTimeDelta)
{
	// 여기에 자동 함수 추가

	return 0;
}

void CSceneComponent::Free()
{
	SUPER::Free();

	Safe_Release(m_pTransformComp);
}

CSceneComponent* CSceneComponent::Get_FirstChildComp()
{
	if (m_listChildSceneComp.empty())
		return nullptr;

	return m_listChildSceneComp.front();
}

CSceneComponent* CSceneComponent::Get_ChildComp(_uint iIndex)
{
	if (m_listChildSceneComp.empty())
		return nullptr;

	_uint i = 0;
	for (auto iter = m_listChildSceneComp.begin(); iter != m_listChildSceneComp.end(); iter++)
	{
		if (i == iIndex)
			return (*iter);
		++i;
	}

	return nullptr;
}

void CSceneComponent::Add_Child(CSceneComponent* const pComp)
{
	if (nullptr == pComp)
		return;

	m_listChildSceneComp.push_back(pComp);
}

_bool CSceneComponent::Insert_Child(_uint iIndex, CSceneComponent* const pComp)
{
	if (nullptr == pComp)
		return false;

	_uint i = 0;
	auto iter = m_listChildSceneComp.begin();
	for (; iter != m_listChildSceneComp.end(); iter++)
	{
		if (i == iIndex)
			break;
		++i;
	}
	if (iter != m_listChildSceneComp.end())
		m_listChildSceneComp.insert(iter, pComp);
	else
		return false;

	return true;
}
