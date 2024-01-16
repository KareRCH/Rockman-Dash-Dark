#include "Component/TransformComponent.h"

#include "Component/EffectComponent.h"
#include <iomanip>

CTransformComponent::CTransformComponent()
	: m_vRotation(_float3(0.f, 0.f, 0.f))
	, m_qtOrientation(_float4(0.f, 0.f, 0.f, 1.f))
	, m_vScale(1.f, 1.f, 1.f)
	, m_matTransform()
{
	XMStoreFloat4x4(&m_matTransform, XMMatrixIdentity());
}

CTransformComponent::CTransformComponent(const CTransformComponent& rhs)
	: Base(rhs)
{
}

HRESULT CTransformComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CTransformComponent::Initialize(void* Arg)
{
	return S_OK;
}

CTransformComponent* CTransformComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CGraphicDev Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CTransformComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("TransformComponent Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CTransformComponent::Free()
{
	SUPER::Free();
}

HRESULT CTransformComponent::Bind_TransformToEffect(CEffectComponent* pEffect, const _char* pConstantName)
{
	return pEffect->Bind_Matrix(pConstantName, &m_matTransform);
}

void CTransformComponent::MoveForward(_float fValue)
{
	_vector vLook = XMVector3Normalize(Get_LookVector());

	Set_Position(Get_PositionVector() + (vLook * fValue));
}

void CTransformComponent::MoveUpward(_float fValue)
{
	_vector vUp = XMVector3Normalize(Get_UpVector());

	Set_Position(Get_PositionVector() + (vUp * fValue));
}

void CTransformComponent::MoveRightward(_float fValue)
{
	_vector vRight = XMVector3Normalize(Get_RightVector());

	Set_Position(Get_PositionVector() + (vRight * fValue));
}

void CTransformComponent::TurnRight(_float fRadian)
{
	_vector vRight = Get_RightVector();
	_vector vUp = Get_UpVector();
	_vector vLook = Get_LookVector();

	_matrix matRotation = XMMatrixRotationAxis(XMVector3Normalize(vUp), fRadian);

	Set_Right(XMVector3TransformNormal(vRight, matRotation));
	Set_Up(XMVector3TransformNormal(vUp, matRotation));
	Set_Look(XMVector3TransformNormal(vLook, matRotation));
}

void CTransformComponent::TurnUp(_float fRadian)
{
	_vector vRight = Get_RightVector();
	_vector vUp = Get_UpVector();
	_vector vLook = Get_LookVector();

	_matrix matRotation = XMMatrixRotationAxis(XMVector3Normalize(vRight), fRadian);

	Set_Right(XMVector3TransformNormal(vRight, matRotation));
	Set_Up(XMVector3TransformNormal(vUp, matRotation));
	Set_Look(XMVector3TransformNormal(vLook, matRotation));
}

void CTransformComponent::TurnLook(_float fRadian)
{
	_vector vRight = Get_RightVector();
	_vector vUp = Get_UpVector();
	_vector vLook = Get_LookVector();

	_matrix matRotation = XMMatrixRotationAxis(XMVector3Normalize(vLook), fRadian);

	Set_Right(XMVector3TransformNormal(vRight, matRotation));
	Set_Up(XMVector3TransformNormal(vUp, matRotation));
	Set_Look(XMVector3TransformNormal(vLook, matRotation));
}

void CTransformComponent::TurnAxis(_float3 vAxis, _float fRadian)
{
	_vector vRight = Get_RightVector();
	_vector vUp = Get_UpVector();
	_vector vLook = Get_LookVector();

	_matrix matRotation = XMMatrixRotationAxis(XMVector3Normalize(XMLoadFloat3(&vAxis)), fRadian);

	Set_Right(XMVector3TransformNormal(vRight, matRotation));
	Set_Up(XMVector3TransformNormal(vUp, matRotation));
	Set_Look(XMVector3TransformNormal(vLook, matRotation));
}

void CTransformComponent::Look_At(_fvector vTargetPos)
{
	_float3		vScale = Get_ScaleFloat3();

	_vector		vPosition = Get_PositionVector();

	_vector		vLook = XMVector3Normalize(vTargetPos - vPosition) * vScale.z;
	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;
	_vector		vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight)) * vScale.y;

	Set_Right(vRight);
	Set_Up(vUp);
	Set_Look(vLook);
}

void CTransformComponent::Look_At_OnLand(_fvector vTargetPos)
{
	_float3		vScale = Get_ScaleFloat3();

	_vector		vPosition = Get_PositionVector();
	_vector		vLook = vTargetPos - vPosition;

	if (XMVector3Equal(vLook, XMVectorSet(0.f, 0.f, 0.f, 0.f)))
		vLook = Get_LookNormalizedVector();

	_vector		vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * vScale.x;

	vLook = XMVector3Normalize(XMVector3Cross(vRight, XMVectorSet(0.f, 1.f, 0.f, 0.f))) * vScale.z;

	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;

	Set_Right(vRight);
	Set_Up(vUp);
	Set_Look(vLook);

	_matrix		vMat = Get_TransformMatrix();
	if (isnan(vMat.r->m128_f32[0]))
	{
		int t = 0;
	}
}

_float CTransformComponent::Look_At_OnLand(_fvector vTargetPos, _float fRadianSpeed)
{
	_float3		vScale = Get_ScaleFloat3();

	_vector		vPosition = Get_PositionVector();
	_vector		vLook = XMVector3Normalize(vTargetPos - vPosition);
	_vector		vOriginLook = Get_LookNormalizedVector();
	
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vUp));

	_float fDot = XMVectorGetX(XMVector3Dot(vLook, vOriginLook));
	_float fDir = XMVectorGetX(XMVector3Dot(vRight, vOriginLook));
	_float fRadian = max(min(acos(fDot), fRadianSpeed), 0.f);
	if (isnan(fRadian))
		fRadian = 0.f;
	_vector vQuaternion = XMQuaternionIdentity();

	if (fDir <= 0.f)
		vQuaternion = XMQuaternionRotationAxis(vUp, fRadian);
	else
		vQuaternion = XMQuaternionRotationAxis(vUp, -fRadian);
	
	vLook = XMVector3Normalize(XMVector3Rotate(vOriginLook, vQuaternion));
	vRight = XMVector3Normalize(XMVector3Cross(vUp, vLook));

	vRight	*= vScale.x;
	vUp		*= vScale.y;
	vLook	*= vScale.z;

	Set_Right(vRight);
	Set_Up(vUp);
	Set_Look(vLook);

	return fRadian;
}


