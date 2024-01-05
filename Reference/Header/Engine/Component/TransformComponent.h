#pragma once

#include "Component/InternalComponent.h"
#include "Component/Interface/ITransform.h"

BEGIN(Engine)

/// <summary>
/// 씬 컴포넌트로부터 상속받은 좌표계를 통해 추가적인
/// 행렬 조작을 할 수 있도록 만들어진 클래스
/// </summary>
class ENGINE_DLL CTransformComponent : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CTransformComponent)
protected:
	explicit CTransformComponent();
	explicit CTransformComponent(const CTransformComponent& rhs);
	virtual ~CTransformComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static	CTransformComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;


public:
	HRESULT Bind_TransformToEffect(class CEffectComponent* pEffect, const _char* pConstantName);


public:
#pragma region Position
	inline const _vector Get_PositionVector()
	{
		return XMVectorSet(m_matTransform._41, m_matTransform._42, m_matTransform._43, m_matTransform._44);
	}
	inline const _float3 Get_PositionFloat3()
	{
		return _float3(m_matTransform._41, m_matTransform._42, m_matTransform._43);
	}
	inline void Set_Position(_fvector vPos)
	{
		m_matTransform._41 = XMVectorGetX(vPos);
		m_matTransform._42 = XMVectorGetY(vPos);
		m_matTransform._43 = XMVectorGetZ(vPos);
	}
	inline void Set_Position(const _float3& vPos)
	{
		m_matTransform._41 = vPos.x;
		m_matTransform._42 = vPos.y;
		m_matTransform._43 = vPos.z;
	}
	inline void Set_Position(const _float x, const _float y, const _float z)
	{
		m_matTransform._41 = x;
		m_matTransform._42 = y;
		m_matTransform._43 = z;
	}
	inline void Set_PositionX(const _float value) { m_matTransform._41 = value; }
	inline void Set_PositionY(const _float value) { m_matTransform._42 = value; }
	inline void Set_PositionZ(const _float value) { m_matTransform._43 = value; }
#pragma endregion




#pragma region Right UP Look
	inline void Set_Right(_fvector vRight)
	{
		m_matTransform.m[0][0] = XMVectorGetX(vRight);
		m_matTransform.m[0][1] = XMVectorGetY(vRight);
		m_matTransform.m[0][2] = XMVectorGetZ(vRight);
		m_matTransform.m[0][3] = XMVectorGetW(vRight);
	}
	inline const _vector Get_RightVector()
	{
		return XMVectorSet(m_matTransform._11, m_matTransform._12, m_matTransform._13, m_matTransform._14);
	}
	inline const _float3 Get_RightFloat3()
	{
		return _float3(m_matTransform._11, m_matTransform._12, m_matTransform._13);
	}

	inline void Set_Up(_fvector vUp)
	{
		m_matTransform.m[1][0] = XMVectorGetX(vUp);
		m_matTransform.m[1][1] = XMVectorGetY(vUp);
		m_matTransform.m[1][2] = XMVectorGetZ(vUp);
		m_matTransform.m[1][3] = XMVectorGetW(vUp);
	}
	inline const _vector Get_UpVector()
	{
		return XMVectorSet(m_matTransform._21, m_matTransform._22, m_matTransform._23, m_matTransform._24);
	}
	inline const _float3 Get_UpFloat3()
	{
		return _float3(m_matTransform._21, m_matTransform._22, m_matTransform._23);
	}

	inline void Set_Look(_fvector vLook)
	{
		m_matTransform.m[2][0] = XMVectorGetX(vLook);
		m_matTransform.m[2][1] = XMVectorGetY(vLook);
		m_matTransform.m[2][2] = XMVectorGetZ(vLook);
		m_matTransform.m[2][3] = XMVectorGetW(vLook);
	}
	inline const _vector Get_LookVector()
	{
		return XMVectorSet(m_matTransform._31, m_matTransform._32, m_matTransform._33, m_matTransform._34);
	}
	inline const _float3 Get_LookFloat3()
	{
		return _float3(m_matTransform._31, m_matTransform._32, m_matTransform._33);
	}
#pragma endregion




#pragma region Rotation
	const _vector Get_RotationEulerVector()
	{
		_matrix matRot = {
			m_matTransform._11, m_matTransform._12, m_matTransform._13, 0,
			m_matTransform._21, m_matTransform._22, m_matTransform._23, 0,
			m_matTransform._31, m_matTransform._32, m_matTransform._33, 0,
			0, 0, 0, 1
		};

		_vector vRot = XMQuaternionRotationMatrix(matRot);
		_float3 vEuler = {};
		_float pitch, yaw, roll;
		_float t0, t1;
		t0 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[2]);
		t1 = 1.f - 2.f * (vRot.m128_f32[0] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[1]);
		pitch = atan2(t0, t1);

		_float t2;
		t2 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[1] - vRot.m128_f32[2] * vRot.m128_f32[0]);
		t2 = (t2 > 1.f) ? 1.f : t2;
		t2 = (t2 < -1.f) ? -1.f : t2;
		yaw = asin(t2);

		_float t3, t4;
		t3 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[2] + vRot.m128_f32[0] * vRot.m128_f32[1]);
		t4 = 1.f - 2.f * (vRot.m128_f32[1] * vRot.m128_f32[1] + vRot.m128_f32[2] * vRot.m128_f32[2]);
		roll = atan2(t3, t4);

		return XMVectorSet(pitch, yaw, roll, 0.f);
	}
	const _float3 Get_RotationEulerFloat3()
	{
		_matrix matRot = {
			m_matTransform._11, m_matTransform._12, m_matTransform._13, 0,
			m_matTransform._21, m_matTransform._22, m_matTransform._23, 0,
			m_matTransform._31, m_matTransform._32, m_matTransform._33, 0,
			0, 0, 0, 1
		};

		_vector vRot = XMQuaternionRotationMatrix(matRot);
		_float3 vEuler = {};
		_float pitch, yaw, roll;
		_float t0, t1;
		t0 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[2]);
		t1 = 1.f - 2.f * (vRot.m128_f32[0] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[1]);
		pitch = atan2(t0, t1);

		_float t2;
		t2 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[1] - vRot.m128_f32[2] * vRot.m128_f32[0]);
		t2 = (t2 > 1.f) ? 1.f : t2;
		t2 = (t2 < -1.f) ? -1.f : t2;
		yaw = asin(t2);

		_float t3, t4;
		t3 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[2] + vRot.m128_f32[0] * vRot.m128_f32[1]);
		t4 = 1.f - 2.f * (vRot.m128_f32[1] * vRot.m128_f32[1] + vRot.m128_f32[2] * vRot.m128_f32[2]);
		roll = atan2(t3, t4);

		return _float3(pitch, yaw, roll);
	}
	void Set_RotationEuler(_fvector vRot)
	{
		_vector vPosition = Get_PositionVector();
		_vector vRotation = XMQuaternionRotationRollPitchYawFromVector(vRot);
		_vector vScale = Get_ScaleVector();

		_matrix matTrans = XMMatrixAffineTransformation(vScale, {}, vRotation, vPosition);

		XMStoreFloat4x4(&m_matTransform, matTrans);
	}
	void Set_RotationEuler(const _float3 vRot)
	{
		Set_RotationEuler(XMLoadFloat3(&vRot));
	}
	void Set_RotationEuler(const _float x, const _float y, const _float z)
	{
		_float3 vRot = _float3(x, y, z);
		Set_RotationEuler(XMLoadFloat3(&vRot));
	}
	void Set_RotationEulerX(const _float x)
	{
		_float3 vRot = Get_RotationEulerFloat3();
		vRot.x = x;
		Set_RotationEuler(XMLoadFloat3(&vRot));
	}
	void Set_RotationEulerY(const _float y)
	{
		_float3 vRot = Get_RotationEulerFloat3();
		vRot.y = y;
		Set_RotationEuler(XMLoadFloat3(&vRot));
	}
	void Set_RotationEulerZ(const _float z)
	{
		_float3 vRot = Get_RotationEulerFloat3();
		vRot.z = z;
		Set_RotationEuler(XMLoadFloat3(&vRot));
	}

	void Set_RotationAxis(_fvector vAxis, _float fRadian)
	{
		_float3 vScale = Get_ScaleFloat3();

		_vector vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
		_vector vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

		_matrix matRotation = XMMatrixRotationAxis(vAxis, fRadian);

		Set_Right(XMVector3TransformNormal(vRight, matRotation));
		Set_Up(XMVector3TransformNormal(vUp, matRotation));
		Set_Look(XMVector3TransformNormal(vLook, matRotation));
	}
	void Set_RotationAxis(const _float3 vAxis, _float fRadian)
	{
		Set_RotationAxis(XMLoadFloat3(&vAxis), fRadian);
	}

	const _vector Get_RotationQuaternionVector() { return _vector(); }
	const _float4 Get_RotationQuaternionFloat4() { return _float4(); }
	void Set_RotationQuaternion(_fvector qtRot)
	{
		_vector vPosition = Get_PositionVector();
		_vector vRotation = qtRot;
		_vector vScale = Get_ScaleVector();

		_matrix matTrans = XMMatrixAffineTransformation(vScale, {}, vRotation, vPosition);

		XMStoreFloat4x4(&m_matTransform, matTrans);
	}
	void Set_RotationQuternion(_float4 qtRot)
	{
		Set_RotationQuaternion(XMLoadFloat4(&qtRot));
	}
#pragma endregion


	

#pragma region Scale
	// 스케일
	_vector Get_ScaleVector()
	{
		return XMVectorSet(m_vScale.x, m_vScale.y, m_vScale.z, 0.f);
	}
	_float3 Get_ScaleFloat3()
	{
		return m_vScale;
	}
	void Set_Scale(_fvector value)
	{
		m_matTransform._11 *= XMVectorGetX(value) / m_vScale.x; m_matTransform._12 *= XMVectorGetX(value) / m_vScale.x; m_matTransform._13 *= XMVectorGetX(value) / m_vScale.x;
		m_matTransform._21 *= XMVectorGetY(value) / m_vScale.y; m_matTransform._22 *= XMVectorGetY(value) / m_vScale.y; m_matTransform._23 *= XMVectorGetY(value) / m_vScale.y;
		m_matTransform._31 *= XMVectorGetZ(value) / m_vScale.z; m_matTransform._32 *= XMVectorGetZ(value) / m_vScale.z; m_matTransform._33 *= XMVectorGetZ(value) / m_vScale.z;

		// 부동소수점 문제로 Scale값은 따로 저장
		XMStoreFloat3(&m_vScale, value);
	}
	void Set_Scale(const _float3 value)
	{
		m_matTransform._11 *= value.x / m_vScale.x; m_matTransform._12 *= value.x / m_vScale.x; m_matTransform._13 *= value.x / m_vScale.x;
		m_matTransform._21 *= value.y / m_vScale.y; m_matTransform._22 *= value.y / m_vScale.y; m_matTransform._23 *= value.y / m_vScale.y;
		m_matTransform._31 *= value.z / m_vScale.z; m_matTransform._32 *= value.z / m_vScale.z; m_matTransform._33 *= value.z / m_vScale.z;

		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale = value;
	}
	void Set_Scale(const _float x, const _float y, const _float z)
	{
		m_matTransform._11 *= x / m_vScale.x; m_matTransform._12 *= x / m_vScale.x; m_matTransform._13 *= x / m_vScale.x;
		m_matTransform._21 *= y / m_vScale.y; m_matTransform._22 *= y / m_vScale.y; m_matTransform._23 *= y / m_vScale.y;
		m_matTransform._31 *= z / m_vScale.z; m_matTransform._32 *= z / m_vScale.z; m_matTransform._33 *= z / m_vScale.z;

		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale = { x, y, z };
	}
	void Set_ScaleX(const _float value)
	{
		m_matTransform._11 *= value / m_vScale.x; m_matTransform._12 *= value / m_vScale.x; m_matTransform._13 *= value / m_vScale.x;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.x = value;
	}
	void Set_ScaleY(const _float value)
	{
		m_matTransform._21 *= value / m_vScale.y; m_matTransform._22 *= value / m_vScale.y; m_matTransform._23 *= value / m_vScale.y;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.y = value;
	}
	void Set_ScaleZ(const _float value)
	{
		m_matTransform._31 *= value / m_vScale.z; m_matTransform._32 *= value / m_vScale.z; m_matTransform._33 *= value / m_vScale.z;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.z = value;
	}
#pragma endregion




#pragma region Transform
	// 트랜스폼
	const _float4x4& Get_TransformFloat4x4() const
	{ return m_matTransform; }
	const _matrix Get_TransformMatrix() const
	{ return XMLoadFloat4x4(&m_matTransform); }
	const _float4x4 Get_TransformInverseFloat4x4() const
	{
		_float4x4 matResult;
		XMStoreFloat4x4(&matResult, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matTransform)));
		return matResult;
	}
	const _matrix Get_TransformInverseMatrix() const
	{
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_matTransform));
	}
	void Set_Transform(_fmatrix matTransform)
	{
		XMStoreFloat4x4(&m_matTransform, matTransform);
	}
	void Set_Transform(const _float4x4& matTransform)
	{
		m_matTransform = matTransform;
	}
#pragma endregion




public:
	// 기타 변환
	// 앞쪽 방향으로 움직이게 하기, 음수면 뒤로 움직임
	void MoveForward(_float fValue);
	// 위쪽 방향으로 움직이게 하기, 음수면 아래로 움직임
	void MoveUpward(_float fValue);
	// 오른쪽 방향으로 움직이게 하기, 음수면 왼쪽으로 움직임
	void MoveRightward(_float fValue);

	// 오른쪽 방향으로 돌게 하기, 음수면 왼쪽이 됨
	void TurnRight(_float fRadian);
	// 위쪽 방향으로 돌게 하기, 음수면 아래쪽이 됨
	void TurnUp(_float fRadian);
	// 원하는 축으로 회전하게 하기
	void TurnAxis(_float3 vAxis, _float fRadian);

	// 특정 위치를 바라보게 만드는 함수
	void Look_At(_fvector vTargetPos);
	// 특정 위치에 대해 Right벡터를 고정하고 보도록하는 함수
	void Look_At_OnLand(_fvector vTargetPos);
	

private:
	_float4		m_qtOrientation;	// 회전을 저장하는 쿼터니언
	_float3		m_vScale;			// 회전에 의해 사이즈가 작아지기에 원형 사이즈가 필요하다.
	_float4x4	m_matTransform;		// 변환된 행렬
};

END