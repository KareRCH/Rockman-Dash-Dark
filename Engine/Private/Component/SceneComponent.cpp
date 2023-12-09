#include "Component/SceneComponent.h"

#include "System/GameInstance.h"

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
	, m_pPipelineComp(rhs.m_pPipelineComp)
{
}

HRESULT CSceneComponent::Initialize_Prototype(void* Arg)
{
	if (S_OK != SUPER::Initialize_Prototype())
		return E_FAIL;

	m_pTransformComp = CTransformComponent::Create();
	m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp"));

	return S_OK;
}

HRESULT CSceneComponent::Initialize(void* Arg)
{
	m_pTransformComp = CTransformComponent::Create();

	return S_OK;
}

void CSceneComponent::Tick(const _float& fTimeDelta)
{
	// 여기에 자동 함수 추가


}

void CSceneComponent::Free()
{
	SUPER::Free();

	Release_Transform();
	Safe_Release(m_pPipelineComp);
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

_matrix CSceneComponent::Calculate_TransformMatrixFromParent()
{
	_matrix matCalc = Transform().Get_TransformMatrix();

	if (m_pParentSceneComp)
		matCalc *= Calculate_TransformMatrixFromParent();
	else if (Get_OwnerObject())
		matCalc *= Get_OwnerObject()->Transform().Get_TransformMatrix();
	
	return matCalc;
}

_float4x4 CSceneComponent::Calculate_TransformFloat4x4FromParent()
{
	_matrix matCalc = Calculate_TransformMatrixFromParent();

	_float4x4 matResult = {};
	XMStoreFloat4x4(&matResult, matCalc);

	return matResult;
}
