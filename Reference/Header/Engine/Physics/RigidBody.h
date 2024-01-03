#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class FCollisionPrimitive;

enum class ERIGID_BODY_TYPE
{
    // 질량 무한대, 움직이지 않는 지형 타입
    STATIC,
    // 물리 시뮬레이션에 종속적인 타입
    DYNAMIC,
    // 힘 발생기를 제외한 회전과 같은 힘에 영향을 받지 않는 타입, 플레이어 정의 물리 사용
    KINEMATIC
};

/// <summary>
/// 강체의 물리 속성을 가지는 클래스. 
/// F접두어는 언리얼에서 쓰이는 순수 클래스 접두어다. 
/// 여기서 모티브 받아 붙인 접두어이다.
/// 여기에서 F클래스는 구조체 처럼 쓰이거나 Base를 상속받지 않는 독립적인 형태의 클래스를 제작할 때 쓰인다.
/// Vector 클래스를 FloatVector, Integer, DoubleVector와 구분하기 위해 붙였다고 하는데...그렇다고 한다.
/// 멤버 변수 규칙은 m_를 붙이지 않습니다.
/// </summary>
class ENGINE_DLL FRigidBody
{
	THIS_CLASS(FRigidBody)

public:
    FRigidBody()
        : eBodyType(ERIGID_BODY_TYPE::STATIC), iID()
        , fInverseMass(_float()), matInverseInertiaTensor()
        , fLinearDamping(_float()), fAngularDamping(_float())
        , vForceAccum(), vTorqueAccum(), vAcceleration(), vLastFrameAcceleration()
        , vPosition(), vVelocity(), vRotation(), qtOrientation()
        , matTransform(), matInverseInertiaTensorWorld()
        , fMotion(_float()), bIsAwake(false), bCanSleep(false)
    {
        XMStoreFloat3x4(&matTransform, XMMatrixIdentity());
    }
    FRigidBody(const FRigidBody& rhs)
        : eBodyType(rhs.eBodyType), iID(rhs.iID)
        , fInverseMass(rhs.fInverseMass), matInverseInertiaTensor(rhs.matInverseInertiaTensor)
        , fLinearDamping(rhs.fLinearDamping), fAngularDamping(rhs.fAngularDamping)
        , vForceAccum(rhs.vForceAccum), vTorqueAccum(rhs.vTorqueAccum), vAcceleration(rhs.vAcceleration), vLastFrameAcceleration(rhs.vLastFrameAcceleration)
        , vPosition(rhs.vPosition), vVelocity(rhs.vVelocity), vRotation(rhs.vRotation), qtOrientation(rhs.qtOrientation)
        , matTransform(rhs.matTransform), matInverseInertiaTensorWorld(rhs.matInverseInertiaTensorWorld)
        , fMotion(rhs.fMotion), bIsAwake(rhs.bIsAwake), bCanSleep(rhs.bCanSleep)
    {}
    ~FRigidBody() {}

public:
    // 적분기
    void CalculateDerivedData();
    void Integrate(const _float& fDuration);

public:
    // 질량 함수
    inline void Set_Mass(const _float fMass)
    {
        assert(fMass != 0);
        fInverseMass = ((_float)1.0) / fMass;
    }
    inline _float Get_Mass() const
    {
        if (fInverseMass == 0) {
            return FLT_MAX;
        }
        else {
            return ((_float)1.0) / fInverseMass;
        }
    }
    inline bool HasFiniteMass() const
    {
        return fInverseMass >= 0.0f;
    }
    // 역질량 함수
    GETSET_2(_float, fInverseMass, InversMass, SET_C, GET)

    // 관성 함수
    inline void Set_InertiaTensor(const _fmatrix& matSimInertiaTensor)
    {
        _matrix matSimInverseInertiaTensor = XMLoadFloat3x3(&matInverseInertiaTensor);
        _vector vDeterminant = XMMatrixDeterminant(matSimInertiaTensor);
        matSimInverseInertiaTensor = XMMatrixInverse(&vDeterminant, matSimInverseInertiaTensor);
        XMStoreFloat3x3(& matInverseInertiaTensor, matSimInverseInertiaTensor);
        //_CheckInverseInertiaTensor(matInverseInertiaTensor);
    }
    inline void Get_InertiaTensor(_float3x3* matInertiaTensor) const
    {
        if (nullptr == matInertiaTensor)
            return;

        _matrix matSimInverseInertiaTensor = XMLoadFloat3x3(&matInverseInertiaTensor);
        _vector vDeterminant = XMMatrixDeterminant(matSimInverseInertiaTensor);
        matSimInverseInertiaTensor = XMMatrixInverse(&vDeterminant, matSimInverseInertiaTensor);

        XMStoreFloat3x3(matInertiaTensor, matSimInverseInertiaTensor);
    }
    inline _float3x3 Get_InertiaTensor() const
    {
        _float3x3 it;
        Get_InertiaTensor(&it);
        return it;
    }
    inline void Get_InertiaTensorWorld(_float3x3* matInertiaTensor) const
    {
        if (nullptr == matInertiaTensor)
            return;

        _matrix matSimInverseInertiaTensor = XMLoadFloat3x3(&matInverseInertiaTensor);
        _vector vDeterminant = XMMatrixDeterminant(matSimInverseInertiaTensor);
        matSimInverseInertiaTensor = XMMatrixInverse(&vDeterminant, matSimInverseInertiaTensor);

        XMStoreFloat3x3(matInertiaTensor, matSimInverseInertiaTensor);
    }
    inline _float3x3 Get_InertiaTensorWorld() const
    {
        return matInverseInertiaTensorWorld;
    }
    
    
    // 역관성 함수
    inline void Set_InverseInertiaTensor(const _float3x3& matInverseInertiaTensor)
    {
        //_CheckInverseInertiaTensor(matInverseInertiaTensor);
        this->matInverseInertiaTensor = matInverseInertiaTensor;
    }
    inline void Get_InverseInertiaTensor(_float3x3* matInverseInertiaTensor) const
    {
        *matInverseInertiaTensor = this->matInverseInertiaTensor;
    }
    inline _float3x3 Get_InverseInertiaTensor() const
    {
        return matInverseInertiaTensor;
    }
    // 역관성 월드 행렬 함수
    inline void Get_InverseInertiaTensorWorld(_float3x3* matInverseInertiaTensor) const
    {
        *matInverseInertiaTensor = matInverseInertiaTensorWorld;
    }
    inline _float3x3 Get_InverseInertiaTensorWorld() const
    {
        return matInverseInertiaTensorWorld;
    }


    // 감쇄 함수
    GETSET_2(_float, fLinearDamping, LinearDamping, SET_C, GET)
    GETSET_2(_float, fAngularDamping, AngularDamping, SET_C, GET)
    inline void Set_Damping(const _float _fLinearDamping, const _float _fAngularDamping)
    {
        fLinearDamping = _fLinearDamping;
        fAngularDamping = _fAngularDamping;
    }
    

    // 위치 함수
    GETSET_2(_float3, vPosition, Position, SET_C_REF, GET)
    inline void Set_Position(const _float x, const _float y, const _float z)
    {
        vPosition.x = x;
        vPosition.y = y;
        vPosition.z = z;
    }
    inline void Get_Position(_float3* _vPosition) const
    {
        *_vPosition = vPosition;
    }


    // 정위 함수
    GETSET_1(_float4, qtOrientation, Orientation, GET)
    inline void Set_Orientation(const _float4& orientation)
    {
        _vector vSimOrientation = XMLoadFloat4(&orientation);
        vSimOrientation = XMQuaternionNormalize(vSimOrientation);
        XMStoreFloat4(&qtOrientation, vSimOrientation);
    }
    inline void Set_Orientation(const _float r, const _float i, const _float j, const _float k)
    {
        _vector vSimOrientation = XMVectorSet(r, i, j, k);
        vSimOrientation = XMQuaternionNormalize(vSimOrientation);
        XMStoreFloat4(&qtOrientation, vSimOrientation);
    }
    inline void Get_Orientation(_float4* orientation) const
    {
        *orientation = qtOrientation;
    }
    inline void Get_Orientation(_float3x3* matrix) const
    {
        Get_Orientation(matrix->m[0]);  // 아래함수에서 처리
    }
    inline void Get_Orientation(_float matrix[9]) const
    {
        // 3x4 -> 3x3
        memcpy(&matrix[0], &matTransform.f[0], sizeof(_float3));
        memcpy(&matrix[3], &matTransform.f[4], sizeof(_float3));
        memcpy(&matrix[6], &matTransform.f[8], sizeof(_float3));
    }

    // 트랜스폼 함수
    _float3x4 Get_TransformFloat3x4()
    {
        return matTransform;
    }
    _matrix Get_TransformMatrix()
    {
        return XMLoadFloat3x4(&matTransform);
    }
    inline void Get_Transform(_float3x4* transform) const
    {
        memcpy(transform, &matTransform, sizeof(_float3x4));
    }
    inline void Get_Transform(_float matrix[16]) const
    {
        // 3x4 -> 4x4
        memcpy(matrix, &matTransform, sizeof(_float) * 12);
        matrix[12] = matrix[13] = matrix[14] = 0;
        matrix[15] = 1;
    }
    inline void Get_GLTransform(_float matrix[16]) const
    {
        matrix[0] = (float)matTransform.f[0];
        matrix[1] = (float)matTransform.f[4];
        matrix[2] = (float)matTransform.f[8];
        matrix[3] = 0;

        matrix[4] = (float)matTransform.f[1];
        matrix[5] = (float)matTransform.f[5];
        matrix[6] = (float)matTransform.f[9];
        matrix[7] = 0;

        matrix[8] = (float)matTransform.f[2];
        matrix[9] = (float)matTransform.f[6];
        matrix[10] = (float)matTransform.f[10];
        matrix[11] = 0;

        matrix[12] = (float)matTransform.f[3];
        matrix[13] = (float)matTransform.f[7];
        matrix[14] = (float)matTransform.f[11];
        matrix[15] = 1;
    }
    inline void Get_DXTransform(_float matrix[16]) const
    {
        matrix[0] = (float)matTransform.f[0];
        matrix[1] = (float)matTransform.f[1];
        matrix[2] = (float)matTransform.f[2];
        matrix[3] = 0;

        matrix[4] = (float)matTransform.f[4];
        matrix[5] = (float)matTransform.f[5];
        matrix[6] = (float)matTransform.f[6];
        matrix[7] = 0;

        matrix[8] = (float)matTransform.f[8];
        matrix[9] = (float)matTransform.f[9];
        matrix[10] = (float)matTransform.f[10];
        matrix[11] = 0;

        matrix[12] = (float)matTransform.f[3];
        matrix[13] = (float)matTransform.f[7];
        matrix[14] = (float)matTransform.f[11];
        matrix[15] = 1;
    }
    // 벡터에 대한 변환 함수
    _float3 Get_PointInLocalSpace(const _float3& vPoint) const
    {
        _matrix matSimTransform = XMLoadFloat3x4(&matTransform);
        _vector vResult = {};
        _float3 vReturn = {};

        matSimTransform = XMMatrixInverse(nullptr, matSimTransform);
        XMVector3TransformCoord(vResult, matSimTransform);
        XMStoreFloat3(&vReturn, vResult);

        return vReturn;
    }
    _float3 Get_PointInWorldSpace(const _float3& vPoint) const
    {
        _vector vSimPoint = XMLoadFloat3(&vPoint);
        _float3 vReturn = {};

        return vReturn;
    }
    _float3 Get_DirectionInLocalSpace(const _float3& vDirection) const
    {
        return _float3();//matTransform.TransformInverseDirection(vDirection);
    }
    _float3 Get_DirectionInWorldSpace(const _float3& vDirection) const
    {
        return _float3();// matTransform.TransformDirection(vDirection);
    }

    // 속도 함수
    GETSET_2(_float3, vVelocity, Velocity, SET_C_REF, GET)
    inline void Set_Velocity(const _float x, const _float y, const _float z)
    {
        vVelocity.x = x; vVelocity.y = y; vVelocity.z = z;
    }
    inline void Get_Velocity(_float3* velocity) const
    {
        *velocity = vVelocity;
    }
    inline void Add_Velocity(const _float3& vDeltaVelocity)
    {
        vVelocity.x += vDeltaVelocity.x;
        vVelocity.y += vDeltaVelocity.y;
        vVelocity.z += vDeltaVelocity.z;
    }


    // 회전 함수
    GETSET_2(_float3, vRotation, Rotation, SET_C_REF, GET)
    inline void Set_Rotation(const _float x, const _float y, const _float z) 
    { vRotation.x = x; vRotation.y = y; vRotation.z = z; }
    inline void Get_Rotation(_float3* rotation) const 
    { *rotation = vRotation; }
    inline void Add_Rotation(const _float3& vDeltaRotation) 
    { 
        vRotation.x += vDeltaRotation.x; 
        vRotation.y += vDeltaRotation.y;
        vRotation.z += vDeltaRotation.z;
    }


    // 동면 함수
    inline bool Get_Awake() const { return bIsAwake; }
    inline void Set_Awake(const bool bWake = true)
    {
        if (bWake) {
            bIsAwake = true;

            // Add a bit of motion to avoid it falling asleep immediately.
            //fMotion = fSleepEpsilon * 2.0f;
        }
        else {
            bIsAwake = false;
            vVelocity = {};
            vRotation = {};
        }
    }
    inline bool Get_CanSleep() const { return bCanSleep; }
    inline void Set_CanSleep(const bool _bCanSleep = true)
    {
        bCanSleep = _bCanSleep;
        if (!_bCanSleep && !bIsAwake)
            Set_Awake();
    }


    // 힘 함수
    inline void Clear_Accumulators()
    {
        vForceAccum = _float3();
        vTorqueAccum = _float3();
    }
    inline void Add_Force(const _float3& vForce)
    {
        vForceAccum.x += vForce.x;
        vForceAccum.y += vForce.y;
        vForceAccum.z += vForce.z;
        bIsAwake = true;
    }
    inline void Add_ForceAtPoint(const _float3& vForce, const _float3& vPoint)
    {
        _vector vSimPT = XMLoadFloat3(&vPoint);
        _vector vSimPosition = XMLoadFloat3(&vPosition);
        _vector vSimForce = XMLoadFloat3(&vForce);
        _vector vSimForceAccum = XMLoadFloat3(&vForceAccum);
        _vector vSimTorqueAccum = XMLoadFloat3(&vTorqueAccum);

        vSimPT -= vSimPosition;
        vSimForceAccum += vSimForce;
        vSimTorqueAccum += XMVector3Cross(vSimPT, vSimForce);   // 외적

        XMStoreFloat3(&vForceAccum, vSimForceAccum);
        XMStoreFloat3(&vTorqueAccum, vSimTorqueAccum);

        bIsAwake = true;
    }
    inline void Add_ForceAtBodyPoint(const _float3& vForce, const _float3& vPoint)
    {
        // 월드 좌표의 점을 얻어 힘 합산기에 적용한다.
        _float3 vPT = Get_PointInWorldSpace(vPoint);
        Add_ForceAtPoint(vForce, vPT);
    }
    inline void Add_Torque(const _float3& vTorque)
    {
        vTorqueAccum.x += vTorque.x;
        vTorqueAccum.y += vTorque.y;
        vTorqueAccum.z += vTorque.z;
        bIsAwake = true;
    }


    // 가속 함수
    GETSET_2(_float3, vAcceleration, Acceleration, SET_C_REF, GET)
    inline void Set_Acceleration(const _float x, const _float y, const _float z)
    {
        vAcceleration.x = x; vAcceleration.y = y; vAcceleration.z = z;
    }
    inline void Get_Acceleration(_float3* pvAcceleration) const
    {
        *pvAcceleration = vAcceleration;
    }
    // 이전 프레임 가속값 얻기
    inline void Get_LastFrameAcceleration(_float3* linearAcceleration) const
    {
        *linearAcceleration = vLastFrameAcceleration;
    }
    inline _float3 Get_LastFrameAcceleration() const
    {
        return vLastFrameAcceleration;
    }
    GETSET_2(ERIGID_BODY_TYPE, eBodyType, BodyType, GET_C_REF, SET_C)
    GETSET_2(unsigned long long, iID, ID, GET_C_REF, SET_C)

protected:
    ERIGID_BODY_TYPE                eBodyType;
    unsigned long long              iID;

protected:
    _float                          fInverseMass;				    // 역질량
	_float3x3                       matInverseInertiaTensor;		// 역관성텐서

    _float                          fLinearDamping;                 // 등속이동 감쇄
    _float                          fAngularDamping;                // 각속도 감쇄
    _float3                         vForceAccum;                    // 속도힘 합
    _float3                         vTorqueAccum;                   // 회전힘 합
    _float3                         vAcceleration;                  // 가속도
    _float3                         vLastFrameAcceleration;         // 이전 프레임 가속도
    
    _float3                         vPosition;                      // 위치
    _float3                         vVelocity;                      // 속도
    _float3                         vRotation;                      // 회전 속도
    _float4                         qtOrientation;                  // 정위, 나중에 쿼터니온으로 바꿀 것
    _float3                         vScale;                         // 크기
    

    _float3x3                       matInverseInertiaTensorWorld;   // 3차원 행렬
    _float3x4                       matTransform;                   // 4차원 행렬, 트랜스폼
    
    _float                          fMotion;                        // 모션
    _bool                           bIsAwake;                       // 힘 작용 업데이트 가능 여부
    _bool                           bCanSleep;                      // 앱실론 자동 연산 제외 기능

public:
    GETSET_2(void*, pOwner, Owner, GET, SET)

protected:
    void*           pOwner = nullptr;                // 충돌기하학 모양, 
};

END