#pragma once

#include "Engine_Define.h"

/// <summary>
/// Transform컴포넌트를 포함하는 객체가 가져야하는 인터페이스
/// 해당 함수들을 구현하여 연결 시켜주어야 한다.
/// </summary>
class ITransform abstract
{
public:
	//// 위치
	//inline virtual _vector Get_PositionVector() PURE;
	//inline virtual _float3 Get_PositionFloat3() PURE;
	//inline virtual void Set_Position(_fvector vPos) PURE;
	//inline virtual void Set_Position(const _float3& vPos) PURE;
	//inline virtual void Set_PositionX(const _float x) PURE;
	//inline virtual void Set_PositionY(const _float y) PURE;
	//inline virtual void Set_PositionZ(const _float z) PURE;
	//
	//// 회전
	//inline virtual _vector Get_RotationEulerVector() PURE;
	//inline virtual _float3 Get_RotationEulerFloat3() PURE;
	//inline virtual void Set_RotationEuler(_fvector vRot) PURE;
	//inline virtual void Set_RotationEuler(const _float3& vRot) PURE;
	//inline virtual void Set_RotationEulerX(const _float x) PURE;
	//inline virtual void Set_RotationEulerY(const _float y) PURE;
	//inline virtual void Set_RotationEulerZ(const _float z) PURE;
	//inline virtual void Set_RotationAxis(_fvector vAxis, _float fRadian) PURE;
	//inline virtual void Set_RotationAxis(_float3 vAxis, _float fRadian) PURE;

	//inline virtual _vector Get_RotationQuaternionVector() PURE;
	//inline virtual _float4 Get_RotationQuaternionFloat4() PURE;
	//inline virtual void Set_RotationQuaternion(_fvector vQuaternion) PURE;
	//inline virtual void Set_RotationQuaternion(const _float4& vQuaternion) PURE;
	//
	//// 크기
	//inline virtual _vector Get_ScaleVector() PURE;
	//inline virtual _float3 Get_ScaleFloat3() PURE;
	//inline virtual void Set_Scale(_fvector vScale) PURE;
	//inline virtual void Set_Scale(const _float3& vScale) PURE;
	//inline virtual void Set_ScaleX(const _float x) PURE;
	//inline virtual void Set_ScaleY(const _float y) PURE;
	//inline virtual void Set_ScaleZ(const _float z) PURE;
	//
	//// 트랜스폼
	//inline virtual _matrix Get_TransformMatrix() PURE;
	//inline virtual const _float4x4& Get_TransformFloat4x4() PURE;
	//inline virtual void Set_Transform(const _float4x4& matTransform) PURE;
	//inline virtual void Set_Transform(_fmatrix matTransform) PURE;

	inline virtual class CTransformComponent& Transform() PURE;
	inline virtual void Release_Transform() PURE;
};

