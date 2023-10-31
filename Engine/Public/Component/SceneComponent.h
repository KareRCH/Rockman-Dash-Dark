#pragma once

#include "Component/PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CSceneComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CSceneComponent)
protected:
	explicit CSceneComponent(ID3D11Device* pGraphicDev);
	explicit CSceneComponent(const CSceneComponent& rhs);
	virtual ~CSceneComponent();

public:
	virtual HRESULT Initialize() PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	LateTick() PURE;
	virtual void	Render(ID3D11DeviceContext* pDeviceContext) PURE;

protected:
	virtual void	Free();

public:
	GETSET_2(_vec3, m_vPosition, Position, GET_C_REF, SET_C_REF)
	void Set_Position(const _float x, const _float y, const _float z)
	{
		m_vPosition.x = x;
		m_vPosition.y = y;
		m_vPosition.z = z;
	}
	void Set_PosX(const _float value) { m_vPosition.x = value; }
	void Set_PosY(const _float value) { m_vPosition.y = value; }
	void Set_PosZ(const _float value) { m_vPosition.z = value; }

	GETSET_2(_vec3, m_vRotation, Rotation, GET_C_REF, SET_C_REF)
	void Set_Rotation(const _float x, const _float y, const _float z)
	{
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	}
	void Set_RotationX(const _float value) { m_vRotation.x = value; }
	void Set_RotationY(const _float value) { m_vRotation.y = value; }
	void Set_RotationZ(const _float value) { m_vRotation.z = value; }

	GETSET_2(_vec3, m_vScale, Scale, GET_C_REF, SET_C_REF)
	void Set_Scale(const _float x, const _float y, const _float z)
	{
		m_vScale.x = x;
		m_vScale.y = y;
		m_vScale.z = z;
	}
	void Set_ScaleX(const _float value) { m_vScale.x = value; }
	void Set_ScaleY(const _float value) { m_vScale.y = value; }
	void Set_ScaleZ(const _float value) { m_vScale.z = value; }

	GETSET_2(_matrix, m_matTransform, Transform, GET_C_REF, SET_C_REF)
	//_vec3 Get_PosByTransform() { return _vec3(m_matTransform.r[3], m_matTransform._42, m_matTransform._43); }

	inline void Calculate_Transform()
	{
		_matrix matPos = XMMatrixTranslationFromVector(XMLoadFloat3(&m_vPosition));
		_matrix matRot = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_vRotation));
		_matrix matScale = XMMatrixScalingFromVector(XMLoadFloat3(&m_vScale));

		m_matTransform = matScale * matRot * matPos;
	}
	inline void Calculate_TransformFromParent(const _matrix& matParent)
	{
		Calculate_Transform();

		m_matTransform *= matParent;
	}

private:
	_vec3		m_vPosition;
	_vec3		m_vRotation;
	_vec3		m_vScale;
	_vec4		m_qtOrientation;
	_matrix		m_matTransform;
};

END