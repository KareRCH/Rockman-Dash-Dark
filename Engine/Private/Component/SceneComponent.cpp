#include "Component/SceneComponent.h"

#include "System/GameInstance.h"

CSceneComponent::CSceneComponent()
{
	// 필수 컴포넌트 내부 생성
	NULL_CHECK(m_pTransformComp = CTransformComponent::Create());
	NULL_CHECK(m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp")))
}

CSceneComponent::CSceneComponent(const CSceneComponent& rhs)
	: Base(rhs)
	, m_pPipelineComp(rhs.m_pPipelineComp)
{
	m_pTransformComp = Cast<CTransformComponent*>(rhs.m_pTransformComp->Clone());
	Safe_AddRef(m_pPipelineComp);
}

HRESULT CSceneComponent::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSceneComponent::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
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

HRESULT CSceneComponent::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CSceneComponent::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
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

void CSceneComponent::Priority_Tick(const _float& fTimeDelta)
{
}

void CSceneComponent::Tick(const _float& fTimeDelta)
{
	// 여기에 자동 함수 추가

}

void CSceneComponent::Late_Tick(const _float& fTimeDelta)
{
}

void CSceneComponent::Free()
{
	SUPER::Free();

	Safe_Release(m_pTransformComp);
	Safe_Release(m_pPipelineComp);
}

FSerialData CSceneComponent::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	_float3 vPos = Transform().Get_PositionFloat3();
	Data.Add_Member("PosX", vPos.x);
	Data.Add_Member("PosY", vPos.y);
	Data.Add_Member("PosZ", vPos.z);

	_float3 vRot = Transform().Get_RotationEulerFloat3();
	Data.Add_Member("RotX", XMConvertToDegrees(vRot.x));
	Data.Add_Member("RotY", XMConvertToDegrees(vRot.y));
	Data.Add_Member("RotZ", XMConvertToDegrees(vRot.z));

	_float3 vScale = Transform().Get_ScaleFloat3();
	Data.Add_Member("ScaleX", vScale.x);
	Data.Add_Member("ScaleY", vScale.y);
	Data.Add_Member("ScaleZ", vScale.z);

	return Data;
}

FSerialData CSceneComponent::SerializeData()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	_float3 vPos = Transform().Get_PositionFloat3();
	Data.Add_Member("PosX", vPos.x);
	Data.Add_Member("PosY", vPos.y);
	Data.Add_Member("PosZ", vPos.z);

	_float3 vRot = Transform().Get_RotationEulerFloat3();
	Data.Add_Member("RotX", XMConvertToDegrees(vRot.x));
	Data.Add_Member("RotY", XMConvertToDegrees(vRot.y));
	Data.Add_Member("RotZ", XMConvertToDegrees(vRot.z));

	_float3 vScale = Transform().Get_ScaleFloat3();
	Data.Add_Member("ScaleX", vScale.x);
	Data.Add_Member("ScaleY", vScale.y);
	Data.Add_Member("ScaleZ", vScale.z);

	return Data;
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

_matrix CSceneComponent::Calculate_TransformMatrixFromParentNoSelf()
{
	_matrix matCalc = XMMatrixIdentity();

	if (m_pParentSceneComp)
		matCalc *= Calculate_TransformMatrixFromParent();
	else if (Get_OwnerObject())
		matCalc *= Get_OwnerObject()->Transform().Get_TransformMatrix();

	return matCalc;
}

_float4x4 CSceneComponent::Calculate_TransformFloat4x4FromParentNoSelf()
{
	_matrix matCalc = Calculate_TransformMatrixFromParentNoSelf();

	_float4x4 matResult = {};
	XMStoreFloat4x4(&matResult, matCalc);

	return matResult;
}
