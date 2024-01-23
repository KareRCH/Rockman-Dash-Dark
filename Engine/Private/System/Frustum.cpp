#include "System/Frustum.h"

#include "System/GameInstance.h"

CFrustum::CFrustum()
	: m_pGI(GI())
{
	Safe_AddRef(m_pGI);
	NULL_CHECK(m_pPipelineComp = Cast<CPipelineComp*>(GI()->Reference_PrototypeComp(L"CamViewComp")));
}

HRESULT CFrustum::Initialize()
{
	/* 투영공간상의 여덟개 점을 정의하자. */
	m_vOriginalPoints[0] = _float3(-1.f, 1.f, 0.f);
	m_vOriginalPoints[1] = _float3(1.f, 1.f, 0.f);
	m_vOriginalPoints[2] = _float3(1.f, -1.f, 0.f);
	m_vOriginalPoints[3] = _float3(-1.f, -1.f, 0.f);

	m_vOriginalPoints[4] = _float3(-1.f, 1.f, 1.f);
	m_vOriginalPoints[5] = _float3(1.f, 1.f, 1.f);
	m_vOriginalPoints[6] = _float3(1.f, -1.f, 1.f);
	m_vOriginalPoints[7] = _float3(-1.f, -1.f, 1.f);

	return S_OK;
}

void CFrustum::Tick()
{
	_matrix		ProjMatrixInv = PipelineComp().Get_CamInvMatrix(ECamType::Persp, ECamMatrix::Proj, ECamNum::One);
	_matrix		ViewMatrixInv = PipelineComp().Get_CamInvMatrix(ECamType::Persp, ECamMatrix::View, ECamNum::One);

	_float3		vPoints[8];

	for (size_t i = 0; i < 8; i++)
	{
		XMStoreFloat3(&vPoints[i], XMVector3TransformCoord(XMLoadFloat3(&m_vOriginalPoints[i]), ProjMatrixInv));
		XMStoreFloat3(&vPoints[i], XMVector3TransformCoord(XMLoadFloat3(&vPoints[i]), ViewMatrixInv));
	}

	Make_Planes(vPoints, m_WorldPlanes);
}

CFrustum* CFrustum::Create()
{
	ThisClass* pInstance = new ThisClass;

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CFrustum");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFrustum::Free()
{
	Safe_Release(m_pGI);
	Safe_Release(m_pPipelineComp);
}

void CFrustum::Transform_ToLocalSpace(_fmatrix WorldMatrix)
{
	_matrix		WorldMatrixTranspose = XMMatrixTranspose(WorldMatrix);

	for (size_t i = 0; i < 6; i++)
	{
		XMStoreFloat4(&m_LocalPlanes[i], XMPlaneTransform(XMLoadFloat4(&m_WorldPlanes[i]), WorldMatrixTranspose));
	}
}

_bool CFrustum::IsIn_WorldPlanes(_fvector vPoint, _float fRadius)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (fRadius < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_WorldPlanes[i]), vPoint)))
			return false;
	}

	return true;
}

_bool CFrustum::IsIn_LocalPlanes(_fvector vPoint, _float fRadius)
{
	for (size_t i = 0; i < 6; i++)
	{
		if (fRadius < XMVectorGetX(XMPlaneDotCoord(XMLoadFloat4(&m_LocalPlanes[i]), vPoint)))
			return false;
	}

	return true;
}

void CFrustum::Make_Planes(const _float3* pPoints, _float4* pPlanes)
{
	XMStoreFloat4(&m_WorldPlanes[0], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[1]),
		XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[6])));

	XMStoreFloat4(&m_WorldPlanes[1], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[0]),
		XMLoadFloat3(&pPoints[3])));

	XMStoreFloat4(&m_WorldPlanes[2], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[1])));

	XMStoreFloat4(&m_WorldPlanes[3], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[3]),
		XMLoadFloat3(&pPoints[2]),
		XMLoadFloat3(&pPoints[6])));

	XMStoreFloat4(&m_WorldPlanes[4], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[5]),
		XMLoadFloat3(&pPoints[4]),
		XMLoadFloat3(&pPoints[7])));

	XMStoreFloat4(&m_WorldPlanes[5], XMPlaneFromPoints(
		XMLoadFloat3(&pPoints[0]),
		XMLoadFloat3(&pPoints[1]),
		XMLoadFloat3(&pPoints[2])));
}
