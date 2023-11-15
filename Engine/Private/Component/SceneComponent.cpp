#include "Component/SceneComponent.h"

CSceneComponent::CSceneComponent(const DX11DEVICE_T tDeivce)
	: Base(tDeivce)
	, m_vPosition(_float3()), m_vRotation(_float3()), m_vScale(_float3(1.f, 1.f, 1.f)), m_qtOrientation(_float4(1.f, 0.f, 0.f, 0.f))
	, m_matTransform(XMMatrixIdentity())
{
	
}

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
{
}

CSceneComponent::~CSceneComponent()
{
}

_int CSceneComponent::Tick(const _float& fTimeDelta)
{
	// 여기에 자동 함수 추가

	return 0;
}

void CSceneComponent::Free()
{
	SUPER::Free();
}

CSceneComponent* CSceneComponent::Get_FirstChildComp()
{
	if (m_listChildrenComp.empty())
		return nullptr;

	return m_listChildrenComp.front();
}

CSceneComponent* CSceneComponent::Get_ChildComp(_uint iIndex)
{
	if (m_listChildrenComp.empty())
		return nullptr;

	_uint i = 0;
	for (auto iter = m_listChildrenComp.begin(); iter != m_listChildrenComp.end(); iter++)
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

	m_listChildrenComp.push_back(pComp);
}

_bool CSceneComponent::Insert_Child(_uint iIndex, CSceneComponent* const pComp)
{
	if (nullptr == pComp)
		return false;

	_uint i = 0;
	auto iter = m_listChildrenComp.begin();
	for (; iter != m_listChildrenComp.end(); iter++)
	{
		if (i == iIndex)
			break;
		++i;
	}
	if (iter != m_listChildrenComp.end())
		m_listChildrenComp.insert(iter, pComp);
	else
		return false;

	return true;
}
