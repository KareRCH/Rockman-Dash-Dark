#pragma once

#include "Component/PrimitiveComponent.h"

BEGIN(Engine)

class ENGINE_DLL CSceneComponent abstract : public CPrimitiveComponent
{
	DERIVED_CLASS(CPrimitiveComponent, CSceneComponent)
protected:
	explicit CSceneComponent(const DX11DEVICE_T tDevice);
	explicit CSceneComponent(const CSceneComponent& rhs);
	virtual ~CSceneComponent();

public:
	virtual HRESULT Initialize() PURE;
	virtual void	Priority_Tick(const _float& fTimeDelta) PURE;
	virtual _int	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta) PURE;
	virtual void	Render() PURE;

protected:
	virtual void	Free();

public:
	GETSET_1(CSceneComponent*, m_pParentComp, ParentComp, GET)
	CSceneComponent*	Get_FirstChildComp();
	CSceneComponent*	Get_ChildComp(_uint iIndex);
	void				Add_Child(CSceneComponent* const pComp);
	_bool				Insert_Child(_uint iIndex, CSceneComponent* const pComp);

protected:
	CSceneComponent*			m_pParentComp = { nullptr };		// 부모 컴포넌트
	list<CSceneComponent*>		m_listChildrenComp;					// 자식 컴포넌트

public:
	GETSET_2(_float3, m_vPosition, Position, GET_C_REF, SET_C_REF)
	void Set_Position(const _float x, const _float y, const _float z)
	{
		m_vPosition.x = x;
		m_vPosition.y = y;
		m_vPosition.z = z;
	}
	void Set_PosX(const _float value) { m_vPosition.x = value; }
	void Set_PosY(const _float value) { m_vPosition.y = value; }
	void Set_PosZ(const _float value) { m_vPosition.z = value; }

	GETSET_2(_float3, m_vRotation, Rotation, GET_C_REF, SET_C_REF)
	void Set_Rotation(const _float x, const _float y, const _float z)
	{
		m_vRotation.x = x;
		m_vRotation.y = y;
		m_vRotation.z = z;
	}
	void Set_RotX(const _float value) { m_vRotation.x = value; }
	void Set_RotY(const _float value) { m_vRotation.y = value; }
	void Set_RotZ(const _float value) { m_vRotation.z = value; }

	GETSET_2(_float3, m_vScale, Scale, GET_C_REF, SET_C_REF)
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
	//_float3 Get_PosByTransform() { return _float3(m_matTransform.r[3], m_matTransform._42, m_matTransform._43); }

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
	inline const _float3 Get_RightFromTransform()
	{
		_float3 vReturn;
		XMStoreFloat3(&vReturn, m_matTransform.r[0]);
		return vReturn;
	}
	inline const _float3 Get_UpFromTransform()
	{
		_float3 vReturn;
		XMStoreFloat3(&vReturn, m_matTransform.r[1]);
		return vReturn;
	}
	inline const _float3 Get_LookFromTransform()
	{
		_float3 vReturn;
		XMStoreFloat3(&vReturn, m_matTransform.r[2]);
		return vReturn;
	}

private:
	_float3		m_vPosition;
	_float3		m_vRotation;
	_float3		m_vScale;
	_float4		m_qtOrientation;
	_matrix		m_matTransform;
};

END