#include "Component/TransformComponent.h"

CTransformComponent::CTransformComponent()
	: m_qtOrientation(_float4(0.f, 0.f, 0.f, 1.f)), m_vScale(1.f, 1.f, 1.f)
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

		return nullptr;
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

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CTransformComponent::Free()
{
	
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


