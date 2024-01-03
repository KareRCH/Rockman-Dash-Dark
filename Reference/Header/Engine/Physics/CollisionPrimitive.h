#pragma once

#include "Engine_Define.h"

#include "Physics/RigidBody.h"
#include "Physics/Contact.h"

#include "Utility/DelegateTemplate.h"

BEGIN(Engine)

enum class ECollisionType
{
	Sphere, Box, Capsule, Plane, Line, Ray, Triangle, OBB
};

struct FBoundingBox
{
	_float3 vMin;
	_float3 vMax;
};

/// <summary>
/// 충돌체
/// 기하학적인 모양을 가지는 클래스
/// 실제 충돌처리 이 클래스를 기반으로 한 파생 클래스를 통해 진행한다.
/// </summary>
class ENGINE_DLL FCollisionPrimitive
{
	THIS_CLASS(FCollisionPrimitive)

public:
	FCollisionPrimitive()
	{
		pBody = new FRigidBody();
		pBody->Set_Owner(this);
		m_dwCollisionLayer_Flag = 0UL;
		m_dwCollisionMask_Flag = 0UL;
	}
	FCollisionPrimitive(const FCollisionPrimitive& rhs)
		: m_dwCollisionLayer_Flag(rhs.m_dwCollisionLayer_Flag)
		, m_dwCollisionMask_Flag(rhs.m_dwCollisionMask_Flag)
		, eType(rhs.eType)
	{
		pBody = new FRigidBody();
		*pBody = *rhs.pBody;
		pBody->Set_Owner(this);
		//arr = nullptr;		// 이벤트 함수는 복사하면 안됨.
	}
	virtual ~FCollisionPrimitive() 
	{
		Safe_Delete(pBody);
	}
	
public:
	GETSET_2(void*, pOwner, Owner, GET, SET)

protected:
	void* pOwner = nullptr;

private:
	// 보안성 코드
	void CheckAnd_CreateBody() const
	{
		if (!pBody) { pBody = new FRigidBody(); }
	}
	void Set_BodyType(const ERIGID_BODY_TYPE eType)
	{
		CheckAnd_CreateBody();
		pBody->Set_BodyType(eType);
	}
	ERIGID_BODY_TYPE Get_BodyType()
	{
		CheckAnd_CreateBody();
		return pBody->Get_BodyType();
	}

public:
	mutable FRigidBody* pBody;		// 강체 정보
	
	_float3x4		matOffset;		// 오프셋 행렬
	FBoundingBox	BoundingBox;	// 브로드 페이즈 용 바운딩 박스

public:
	virtual void Calculate_Shape() PURE;

public:
	void Calculate_Transform()
	{
		XMStoreFloat3x4(&matTransform, pBody->Get_TransformMatrix() * XMLoadFloat3x4(&matOffset));
	}

public:
	GETSET_2(_float3x4, matTransform, Transform, GET_C_REF, GET_REF)
	// 축 정보를 트랜스폼에서 얻어옴
	_float3 Get_Axis(_uint iIndex) const
	{
		return _float3(matTransform.m[iIndex][0], matTransform.m[iIndex][1], matTransform.m[iIndex][2]);
	}

	const _float3 Get_PositionFloat3() const
	{
		return _float3(matTransform.m[0][3], matTransform.m[1][3], matTransform.m[2][3]);
	}
	_vector Get_PositionVector() const
	{
		_float3 vReturn(matTransform.m[0][3], matTransform.m[1][3], matTransform.m[2][3]);
		return XMLoadFloat3(&vReturn);
	}
	void Set_Position(const _float3 vPos)
	{
		CheckAnd_CreateBody();
		pBody->Set_Position(vPos);
	}

	const _float3 Get_Rotation() const
	{
		CheckAnd_CreateBody();
		return pBody->Get_Rotation();
	}
	void Set_Rotation(const _float3 vRot)
	{
		CheckAnd_CreateBody();
		pBody->Set_Rotation(vRot);
	}

	
	_float3 Get_ScaleFloat3() const
	{
		_float3 vReturn = {};
		_matrix matCalc = XMLoadFloat3x4(&matTransform);
		_vector vCalc[3] = { matCalc.r[0], matCalc.r[1], matCalc.r[2] };
		vReturn.x = XMVectorGetX(XMVector3Length(vCalc[0]));
		vReturn.y = XMVectorGetX(XMVector3Length(vCalc[1]));
		vReturn.z = XMVectorGetX(XMVector3Length(vCalc[2]));

		return vReturn;
	}

	_vector Get_ScaleVector() const
	{
		_vector vSimReturn = {};
		_matrix matCalc = XMLoadFloat3x4(&matTransform);
		_vector vCalc[3] = { matCalc.r[0], matCalc.r[1], matCalc.r[2] };
		vSimReturn = XMVectorSet(XMVectorGetX(XMVector3Length(vCalc[0]))
					, XMVectorGetX(XMVector3Length(vCalc[1]))
					, XMVectorGetX(XMVector3Length(vCalc[2])), 0.f);

		return vSimReturn;
	}

protected:
	_float3x4	matTransform;		// 트랜스 폼 행렬

public:
	GETSET_2(_ulong, m_dwCollisionLayer_Flag, CollisionLayer, GET_C_REF, SET_C)
	GETSET_2(_ulong, m_dwCollisionMask_Flag, CollisionMask, GET_C_REF, SET_C)

protected:
	_ulong				m_dwCollisionLayer_Flag;			// 콜리전 레이어, 충돌체가 존재하는 층
	_ulong				m_dwCollisionMask_Flag;				// 콜리전 마스크, 충돌체가 충돌하고 싶어하는 층

public:
	GETSET_2(ECollisionType, eType, Type, GET_C_REF, GET_REF)

protected:
	ECollisionType	eType;				// 타입

public:
	typedef FastDelegate2<void*, const FContact*>	CollisionDelegate;
	void Set_CollisionEvent(CollisionDelegate Event) { EventHandler = Event; }
	void Handle_CollsionEvent(void* pDst, const FContact* pContact) { if (!EventHandler.empty()) EventHandler(pDst, pContact); }

protected:
	CollisionDelegate EventHandler;
};


/// <summary>
/// 구체
/// </summary>
class ENGINE_DLL FCollisionSphere : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionSphere)

public:
	FCollisionSphere()
		: fRadius(0.5f)
	{
		eType = ECollisionType::Sphere;
	}
	FCollisionSphere(const FCollisionSphere& rhs)
		: Base(rhs)
		, fRadius(rhs.fRadius)
	{
	}
	virtual ~FCollisionSphere() {}

public:
	virtual void Calculate_Shape() override
	{
		_float3 vScale = Get_ScaleFloat3();
		fRadius = max(max(vScale.x, vScale.y), vScale.z) * 0.5f;
		_vector vSimLength = XMVectorSet(fRadius, fRadius, fRadius, fRadius);
		XMStoreFloat3(&BoundingBox.vMin,(Get_PositionVector() - vSimLength));
		XMStoreFloat3(&BoundingBox.vMax,(Get_PositionVector() + vSimLength));
	}

public:
	_float		fRadius;
};


/// <summary>
/// 박스
/// </summary>
class ENGINE_DLL FCollisionBox : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionBox)
public:
	FCollisionBox()
		: vHalfSize(0.5f, 0.5f, 0.5f)
	{
		eType = ECollisionType::Box;
	}
	FCollisionBox(const FCollisionBox& rhs)
		: Base(rhs)
		, vHalfSize(rhs.vHalfSize)
	{
	}
	virtual ~FCollisionBox() {}

public:
	virtual void Calculate_Shape() override
	{
		_vector vSimHalfSize = XMLoadFloat3(&vHalfSize);
		vSimHalfSize = Get_ScaleVector() * 0.5f;
		XMStoreFloat3(&vHalfSize, vSimHalfSize);

		XMStoreFloat3(&BoundingBox.vMin, Get_PositionVector() - vSimHalfSize);
		XMStoreFloat3(&BoundingBox.vMax, Get_PositionVector() + vSimHalfSize);
	}

public:
	_float3	vHalfSize;
};


/// <summary>
/// 캡슐
/// </summary>
class ENGINE_DLL FCollisionCapsule : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionCapsule)
public:
	FCollisionCapsule()
		: vDirHalfSize(0.f, 1.f, 0.f), fRadius(0.5f)
	{
		eType = ECollisionType::Capsule;
	}
	FCollisionCapsule(const FCollisionCapsule& rhs)
		: Base(rhs)
		, vDirHalfSize(rhs.vDirHalfSize), fRadius(rhs.fRadius)
	{
	}
	virtual ~FCollisionCapsule() {}

public:
	virtual void Calculate_Shape() override
	{
		vDirHalfSize = _float3(0.f, Get_ScaleFloat3().y * 0.5f, 0.f);
		fRadius = max(Get_ScaleFloat3().x, Get_ScaleFloat3().z) * 0.5f;
		_vector vLength = XMVectorSet(fRadius, vDirHalfSize.y + fRadius, fRadius, 0.f);

		XMStoreFloat3(&BoundingBox.vMin, Get_PositionVector() - vLength);
		XMStoreFloat3(&BoundingBox.vMax, Get_PositionVector() + vLength);
	}

public:
	_float3	vDirHalfSize;
	_float		fRadius;
};


/// <summary>
/// 평면
/// 평면은 강체 정보를 가지지 않는다.
/// </summary>
class ENGINE_DLL FCollisionPlane : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionPlane)
public:
	FCollisionPlane()
		: vDirection(0.f, 1.f, 0.f), fOffset(1.f)
	{
		eType = ECollisionType::Plane;
	}
	FCollisionPlane(const FCollisionPlane& rhs)
		: Base(rhs)
		, vDirection(rhs.vDirection), fOffset(rhs.fOffset)
	{
	}
	virtual ~FCollisionPlane() {}

public:
	virtual void Calculate_Shape() override
	{
		//vHalfSize = matOffset.Get_ScaleVector() * 0.5f;
	}

public:
	_float3	vDirection;
	_float		fOffset;
};


/// <summary>
/// 선
/// </summary>
class ENGINE_DLL FCollisionLine : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionLine)
public:
	FCollisionLine()
		: vStart(0.f, 0.f, 0.f), vEnd(0.f, 1.f, 0.f)
	{
		eType = ECollisionType::Line;
	}
	FCollisionLine(const FCollisionLine& rhs)
		: Base(rhs)
		, vStart(rhs.vStart), vEnd(rhs.vEnd)
	{
	}
	virtual ~FCollisionLine() {}

public:
	virtual void Calculate_Shape() override
	{
		//vHalfSize = matOffset.Get_ScaleVector() * 0.5f;
	}

public:
	_float3	vStart;
	_float3	vEnd;
};

/// <summary>
/// 레이
/// </summary>
class ENGINE_DLL FCollisionRay : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionRay)
public:
	FCollisionRay()
		: vOrigin(0.f, 0.f, 0.f), vDir(0.f, 1.f, 0.f)
	{
		eType = ECollisionType::Ray;
	}
	FCollisionRay(const FCollisionRay& rhs)
		: Base(rhs)
		, vOrigin(rhs.vOrigin), vDir(rhs.vDir)
	{
	}
	virtual ~FCollisionRay() {}

public:
	virtual void Calculate_Shape() override
	{
		//vHalfSize = matOffset.Get_ScaleVector() * 0.5f;
	}

public:
	_float3	vOrigin;
	_float3	vDir;
};

/// <summary>
/// 삼각형
/// </summary>
class ENGINE_DLL FCollisionTriangle : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionTriangle)
public:
	FCollisionTriangle()
		: vA(-0.5f, 0.f, -0.5f), vB(-0.5f, 0.f, 0.5f), vC(0.5f, 0.f, 0.5f)
	{
		eType = ECollisionType::Triangle;
	}
	FCollisionTriangle(const FCollisionTriangle& rhs)
		: Base(rhs)
		, vA(rhs.vA), vB(rhs.vB), vC(rhs.vC)
	{
	}
	virtual ~FCollisionTriangle() {}

public:
	virtual void Calculate_Shape() override
	{
		//vHalfSize = matOffset.Get_ScaleVector() * 0.5f;
	}

public:
	_float3	vA, vB, vC;
};

/// <summary>
/// Oriented Bounding Box
/// </summary>
class ENGINE_DLL FCollisionOBB : public FCollisionPrimitive
{
	DERIVED_CLASS(FCollisionPrimitive, FCollisionOBB)
public:
	FCollisionOBB()
		: vHalfSize(0.5f, 0.5f, 0.5f)
	{
		eType = ECollisionType::OBB;
	}
	FCollisionOBB(const FCollisionOBB& rhs)
		: Base(rhs)
		, vHalfSize(rhs.vHalfSize)
	{
	}
	virtual ~FCollisionOBB() {}

public:
	virtual void Calculate_Shape() override
	{
		_vector vSimHalfSize = XMLoadFloat3(&vHalfSize);
		vSimHalfSize = Get_ScaleVector() * 0.5f;
		XMStoreFloat3(&vHalfSize, vSimHalfSize);

		XMStoreFloat3(&BoundingBox.vMin, Get_PositionVector() - vSimHalfSize);
		XMStoreFloat3(&BoundingBox.vMax, Get_PositionVector() + vSimHalfSize);

		_vector vSize = XMVector3Length(vSimHalfSize);
		XMStoreFloat3(&BoundingBox.vMax, Get_PositionVector() + vSize);
		XMStoreFloat3(&BoundingBox.vMin, Get_PositionVector() - vSize);
	}

public:
	_float3	vHalfSize;
};

END