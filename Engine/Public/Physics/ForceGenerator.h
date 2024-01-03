#pragma once

#include "Engine_Define.h"

#include "Physics/RigidBody.h"

BEGIN(Engine)

class FForceGenerator;

/// <summary>
/// 힘 등록기
/// </summary>
class ENGINE_DLL FForceRegistry
{
protected:
    struct FForceRegistration
    {
        FRigidBody* pBody;
        FForceGenerator* pFGen;
    };
    typedef vector<FForceRegistration> Registry;
    Registry vecRegistrations;

public:
    void Add(FRigidBody* pBody, FForceGenerator* pFGen);
    void Remove(FRigidBody* pBody, FForceGenerator* pFGen);
    void Clear();
    void Update_Forces(const _float fDuration);
};


/// <summary>
/// 각종 힘을 발생시키는 클래스의 원형
/// </summary>
class ENGINE_DLL FForceGenerator abstract
{
	THIS_CLASS(FForceGenerator)

public:
	virtual void Update_Force(FRigidBody* pBody, const _float& fDuration) PURE;

};


/// <summary>
/// 중력 힘 발생기
/// </summary>
class ENGINE_DLL FForce_Gravity
{
	THIS_CLASS(FForce_Gravity)

private:
	_float3 vGravity;

public:
	FForce_Gravity(const _float3& gravity);

	virtual void Update_Force(FRigidBody* pBody, const _float& fDuration);
};


/// <summary>
/// 스프링 힘 발생기
/// </summary>
class ENGINE_DLL FForce_Spring
{
	THIS_CLASS(FForce_Spring)

private:
	_float3 vConnectionPoint;		// 연결 포인트
	_float3 vOtherConnectionPoint;	// 다른 객체 연결 포인트

	FRigidBody* pOther;				// 다른 객체

	_float		fSpringConstant;	// 스프링 상수
	_float		fRestLength;		// 유휴 길이, 원본 길이

public:
	FForce_Spring(const _float3& localConnectionPt, FRigidBody* other, const _float3& otherConnectionPt,
						_float springConstant, _float restLength);

	virtual void Update_Force(FRigidBody* pBody, const _float& fDuration);
};

END