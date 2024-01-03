#pragma once

#include "Engine_Define.h"

#include "Physics/PhysicsCore.h"
#include "Physics/PhysicsPrecision.h"
#include "Physics/RigidBody.h"

BEGIN(Engine)


/// <summary>
/// 접촉 보고서
/// </summary>
class ENGINE_DLL FContact
{
	THIS_CLASS(FContact)

public:
	FRigidBody* pBody[2];           // 충돌 몸체

	_float		fFriction;          // 마찰 계수
    _float		fRestitution;       // 반발 계수

	_float3	    vContactPoint;      // 접촉지점
    _float3	    vContactNormal;     // 접촉법선
    _float      fPenetration;       // 관통 깊이

protected:
    _float3x3    matContactToWorld;
    _float3x3    matContactVelocity;
    _float      fDesiredDeltaVelocity;
    _float3     vRelativeContactPosition[2];

public:
    

    void Set_BodyData(FRigidBody* pSrcBody, FRigidBody* pDstBody,
        _float _fFriction, _float _fRestitution)
    {
        pBody[0] = pSrcBody;
        pBody[1] = pDstBody;
        fFriction = _fFriction;
        fRestitution = _fRestitution;
    }

    void Reverse_BodyData()
    {
        FRigidBody* pTempBody = pBody[0];
        pBody[0] = pBody[1];
        pBody[1] = pBody[0];
        vContactNormal.x = -vContactNormal.x;
        vContactNormal.y = -vContactNormal.y;
        vContactNormal.z = -vContactNormal.z;
    }
};





/// <summary>
/// 접촉 해결
/// </summary>
class ENGINE_DLL FContactResolver
{
	THIS_CLASS(FContactResolver)

protected:
	_uint	iVelocityIterations;        // 속도 정보 반복자개수
	_uint	iPositionIterations;        // 위치 정보 반복자개수
	Real	fVelocityEpsilon;			// 속도 오차
	Real	fPositionEpsilon;			// 위치 오차

public:
	_uint	iVelocityIterationsUsed;    // 사용된 속도 반복자
	_uint	iPositionIterationsUsed;    // 사용된 위치 반복자

private:
	bool	bValidSettings;				// 유효한 세팅인지

public:
    FContactResolver() {}
    FContactResolver(_uint iIterations, Real fVelocityEpsilon = (Real)0.01, Real fPositionEpsilon = (Real)0.01);
    FContactResolver(_uint iVelocityIterations, _uint iPositionIterations, Real fVelocityEpsilon = (Real)0.01, Real fPositionEpsilon = (Real)0.01);
    
    // 유효 체크
    bool IsValid()
    {
        return (iVelocityIterations > 0) &&
            (iPositionIterations > 0) &&
            (fVelocityEpsilon >= 0.0f) &&
            (fPositionEpsilon >= 0.0f);
    }

    // 반복 세팅
    void Set_Iterations(_uint _iVelocityIterations, _uint _iPositionIterations)
    {
        iVelocityIterations = _iVelocityIterations;
        iPositionIterations = _iPositionIterations;
    }
    void Set_Iterations(_uint iIterations)
    {
        Set_Iterations(iIterations, iIterations);
    }
    // 오차 세팅
    void Set_Epsilon(Real _fVelocityEpsilon, Real _fPositionEpsilon)
    {
        fVelocityEpsilon = _fVelocityEpsilon;
        fPositionEpsilon = _fPositionEpsilon;
    }
    void ResolveContacts(FContact* pContactArray, const _uint iContactCount, const Real& fDuration)
    {
        if (iContactCount == 0) return;
        if (IsValid()) return;

        // 접촉 준비
        PrepareContacts(pContactArray, iContactCount, fDuration);

        // 위치 조절
        AdjustPositions(pContactArray, iContactCount, fDuration);

        // 속도 조절
        AdjustVelocities(pContactArray, iContactCount, fDuration);
    }

protected:
    // 접촉 준비
    void PrepareContacts(FContact* pContactArray, _uint iContactCount, Real fDuration)
    {
        // 포인터 기법을 사용한 부분이이 주의하고 볼것.
        FContact* pLastContact = pContactArray + iContactCount;
        for (FContact* pContact = pContactArray; pContact < pLastContact; pContact++)
        {
            //pContact->C
        }
    }
    // 속도, 위치 조절
    void AdjustVelocities(FContact* pContactArray, _uint iContacts, Real fDuration);
    void AdjustPositions(FContact* pContacts, _uint iContacts, Real fDuration);
};


/// <summary>
/// 특수한 충돌에 쓰이는 추상 클래스
/// 접촉에 대해 발생시킨다.
/// </summary>
class ENGINE_DLL FContactGenerator abstract
{
	THIS_CLASS(FContactGenerator)

public:
	virtual _uint Add_Contact(FContact* pContact, _uint iLimit) const PURE;
};

END