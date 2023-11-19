#pragma once

#include "Component/InternalComponent.h"

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
	virtual HRESULT Initialize();

public:
	static	CTransformComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free();

#pragma region Transform
public:
	const _float3 Get_Position()
	{
		return _float3(m_matTransform._41, m_matTransform._42, m_matTransform._43);
	}
	void Set_Position(const _float3 vPos)
	{
		m_matTransform._41 = vPos.x;
		m_matTransform._42 = vPos.y;
		m_matTransform._43 = vPos.z;
	}
	void Set_Position(const _float x, const _float y, const _float z)
	{
		m_matTransform._41 = x;
		m_matTransform._42 = y;
		m_matTransform._43 = z;
	}
	void Set_PosX(const _float value) { m_matTransform._41 = value; }
	void Set_PosY(const _float value) { m_matTransform._42 = value; }
	void Set_PosZ(const _float value) { m_matTransform._43 = value; }

	inline const _float3 Get_Right()
	{
		return _float3(m_matTransform._11, m_matTransform._12, m_matTransform._13);
	}
	inline const _float3 Get_Up()
	{
		return _float3(m_matTransform._21, m_matTransform._22, m_matTransform._23);
	}
	inline const _float3 Get_Look()
	{
		return _float3(m_matTransform._31, m_matTransform._32, m_matTransform._33);
	}


	const _float3 Get_Rotation()
	{
		_matrix matRot = {
			m_matTransform._11, m_matTransform._12, m_matTransform._13, 0,
			m_matTransform._21, m_matTransform._22, m_matTransform._23, 0,
			m_matTransform._31, m_matTransform._32, m_matTransform._33, 0,
			0, 0, 0, 1
		};

		_vec vRot = XMQuaternionRotationMatrix(matRot);
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
	void Set_Rotation(_float3 vRot)
	{
		_matrix matTrans = XMLoadFloat4x4(&m_matTransform);

		_vec vPosition, vRotation, vScale, vEmptyRot = {};
		_float3 vPos = Get_Position();
		vPosition = XMLoadFloat3(&vPos);
		vRotation = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&vRot));
		vScale = XMLoadFloat3(&m_vScale);
		matTrans = XMMatrixAffineTransformation(vScale, vEmptyRot, vRotation, vPosition);

		XMStoreFloat4x4(&m_matTransform, matTrans);
	}
	/*void Set_Rotation(const _float x, const _float y, const _float z)
	{
		_float3();
	}
	void Set_RotX(const _float value) { m_vRotation.x = value; }
	void Set_RotY(const _float value) { m_vRotation.y = value; }
	void Set_RotZ(const _float value) { m_vRotation.z = value; }*/


	_float3 Get_Scale()
	{
		return m_vScale;
	}
	void Set_Scale(_float3 value)
	{
		m_matTransform._11 *= value.x * m_vScale.x; m_matTransform._12 *= value.x * m_vScale.x; m_matTransform._13 *= value.x * m_vScale.x;
		m_matTransform._21 *= value.y * m_vScale.y; m_matTransform._22 *= value.y * m_vScale.y; m_matTransform._23 *= value.y * m_vScale.y;
		m_matTransform._31 *= value.z * m_vScale.z; m_matTransform._32 *= value.z * m_vScale.z; m_matTransform._33 *= value.z * m_vScale.z;

		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale = value;
	}
	void Set_Scale(_float x, _float y, _float z)
	{
		m_matTransform._11 *= x * m_vScale.x; m_matTransform._12 *= x * m_vScale.x; m_matTransform._13 *= x * m_vScale.x;
		m_matTransform._21 *= y * m_vScale.y; m_matTransform._22 *= y * m_vScale.y; m_matTransform._23 *= y * m_vScale.y;
		m_matTransform._31 *= z * m_vScale.z; m_matTransform._32 *= z * m_vScale.z; m_matTransform._33 *= z * m_vScale.z;

		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale = { x, y, z };
	}
	void Set_ScaleX(_float value)
	{
		m_matTransform._11 *= value * m_vScale.x; m_matTransform._12 *= value * m_vScale.x; m_matTransform._13 *= value * m_vScale.x;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.x = value;
	}
	void Set_ScaleY(_float value)
	{
		m_matTransform._21 *= value * m_vScale.y; m_matTransform._22 *= value * m_vScale.y; m_matTransform._23 *= value * m_vScale.y;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.y = value;
	}
	void Set_ScaleZ(_float value)
	{
		m_matTransform._31 *= value * m_vScale.z; m_matTransform._32 *= value * m_vScale.z; m_matTransform._33 *= value * m_vScale.z;
		// 부동소수점 문제로 Scale값은 따로 저장
		m_vScale.z = value;
	}

	GETSET_2(_float4x4, m_matTransform, Transform, GET_C_PTR, SET_C_REF)
		_matrix Get_TransformToCalc()
	{
		return XMLoadFloat4x4(&m_matTransform);
	}
	//_float3 Get_PosByTransform() { return _float3(m_matTransform.r[3], m_matTransform._42, m_matTransform._43); }
	inline void Calculate_TransformFromParent(_matrix* const matParent)
	{
		_matrix matTransform = XMLoadFloat4x4(&m_matTransform);
		matTransform *= *matParent;

		XMStoreFloat4x4(&m_matTransform, matTransform);
	}
#pragma endregion

private:
	_float4		m_qtOrientation;	// 회전을 저장하는 쿼터니언
	_float3		m_vScale;			// 회전에 의해 사이즈가 작아지기에 원형 사이즈가 필요하다.
	_float4x4	m_matTransform;		// 변환된 행렬
};

END