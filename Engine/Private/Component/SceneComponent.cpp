#include "Component/SceneComponent.h"

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
{
}

HRESULT CSceneComponent::Initialize_Prototype(void* Arg)
{
	if (S_OK != SUPER::Initialize_Prototype())
		return E_FAIL;

	m_pTransformComp = CTransformComponent::Create();

	return S_OK;
}

HRESULT CSceneComponent::Initialize(void* Arg)
{
	m_pTransformComp = CTransformComponent::Create();

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

	Release_Transform();
}

CSceneComponent* CSceneComponent::Get_FirstChildSceneComp()
{
	if (m_vecChildSceneComp.empty())
		return nullptr;

	return m_vecChildSceneComp.front();
}

CSceneComponent* CSceneComponent::Get_ChildSceneComp(_uint iIndex)
{
	if (m_vecChildSceneComp.empty())
		return nullptr;

	if (iIndex < 0 || iIndex >= Cast<_uint>(m_vecChildSceneComp.size()))
		return nullptr;

	return m_vecChildSceneComp[iIndex];
}

void CSceneComponent::Add_ChildSceneComp(CSceneComponent* const pComp)
{
	if (nullptr == pComp)
		return;

	m_vecChildSceneComp.push_back(pComp);
}

_bool CSceneComponent::Insert_ChildSceneComp(_uint iIndex, CSceneComponent* const pComp)
{
	if (nullptr == pComp)
		return false;

	if (iIndex < 0 || iIndex >= Cast<_uint>(m_vecChildSceneComp.size()))
		return false;

	auto iter = m_vecChildSceneComp.begin() + iIndex;
	m_vecChildSceneComp.insert(iter, pComp);

	return true;
}
