#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class FCollisionPrimitive;

enum class ERIGID_BODY_TYPE
{
    // ���� ���Ѵ�, �������� �ʴ� ���� Ÿ��
    STATIC,
    // ���� �ùķ��̼ǿ� �������� Ÿ��
    DYNAMIC,
    // �� �߻��⸦ ������ ȸ���� ���� ���� ������ ���� �ʴ� Ÿ��, �÷��̾� ���� ���� ���
    KINEMATIC
};

/// <summary>
/// ��ü�� ���� �Ӽ��� ������ Ŭ����. 
/// F���ξ�� �𸮾󿡼� ���̴� ���� Ŭ���� ���ξ��. 
/// ���⼭ ��Ƽ�� �޾� ���� ���ξ��̴�.
/// ���⿡�� FŬ������ ����ü ó�� ���̰ų� Base�� ��ӹ��� �ʴ� �������� ������ Ŭ������ ������ �� ���δ�.
/// Vector Ŭ������ FloatVector, Integer, DoubleVector�� �����ϱ� ���� �ٿ��ٰ� �ϴµ�...�׷��ٰ� �Ѵ�.
/// ��� ���� ��Ģ�� m_�� ������ �ʽ��ϴ�.
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
    // ���б�
    void CalculateDerivedData();
    void Integrate(const _float& fDuration);

public:
    // ���� �Լ�
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
    // ������ �Լ�
    GETSET_2(_float, fInverseMass, InversMass, SET_C, GET)

    // ���� �Լ�
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
    
    
    // ������ �Լ�
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
    // ������ ���� ��� �Լ�
    inline void Get_InverseInertiaTensorWorld(_float3x3* matInverseInertiaTensor) const
    {
        *matInverseInertiaTensor = matInverseInertiaTensorWorld;
    }
    inline _float3x3 Get_InverseInertiaTensorWorld() const
    {
        return matInverseInertiaTensorWorld;
    }


    // ���� �Լ�
    GETSET_2(_float, fLinearDamping, LinearDamping, SET_C, GET)
    GETSET_2(_float, fAngularDamping, AngularDamping, SET_C, GET)
    inline void Set_Damping(const _float _fLinearDamping, const _float _fAngularDamping)
    {
        fLinearDamping = _fLinearDamping;
        fAngularDamping = _fAngularDamping;
    }
    

    // ��ġ �Լ�
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


    // ���� �Լ�
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
        Get_Orientation(matrix->m[0]);  // �Ʒ��Լ����� ó��
    }
    inline void Get_Orientation(_float matrix[9]) const
    {
        // 3x4 -> 3x3
        memcpy(&matrix[0], &matTransform.f[0], sizeof(_float3));
        memcpy(&matrix[3], &matTransform.f[4], sizeof(_float3));
        memcpy(&matrix[6], &matTransform.f[8], sizeof(_float3));
    }

    // Ʈ������ �Լ�
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
    // ���Ϳ� ���� ��ȯ �Լ�
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

    // �ӵ� �Լ�
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


    // ȸ�� �Լ�
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


    // ���� �Լ�
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


    // �� �Լ�
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
        vSimTorqueAccum += XMVector3Cross(vSimPT, vSimForce);   // ����

        XMStoreFloat3(&vForceAccum, vSimForceAccum);
        XMStoreFloat3(&vTorqueAccum, vSimTorqueAccum);

        bIsAwake = true;
    }
    inline void Add_ForceAtBodyPoint(const _float3& vForce, const _float3& vPoint)
    {
        // ���� ��ǥ�� ���� ��� �� �ջ�⿡ �����Ѵ�.
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


    // ���� �Լ�
    GETSET_2(_float3, vAcceleration, Acceleration, SET_C_REF, GET)
    inline void Set_Acceleration(const _float x, const _float y, const _float z)
    {
        vAcceleration.x = x; vAcceleration.y = y; vAcceleration.z = z;
    }
    inline void Get_Acceleration(_float3* pvAcceleration) const
    {
        *pvAcceleration = vAcceleration;
    }
    // ���� ������ ���Ӱ� ���
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
    _float                          fInverseMass;				    // ������
	_float3x3                       matInverseInertiaTensor;		// �������ټ�

    _float                          fLinearDamping;                 // ����̵� ����
    _float                          fAngularDamping;                // ���ӵ� ����
    _float3                         vForceAccum;                    // �ӵ��� ��
    _float3                         vTorqueAccum;                   // ȸ���� ��
    _float3                         vAcceleration;                  // ���ӵ�
    _float3                         vLastFrameAcceleration;         // ���� ������ ���ӵ�
    
    _float3                         vPosition;                      // ��ġ
    _float3                         vVelocity;                      // �ӵ�
    _float3                         vRotation;                      // ȸ�� �ӵ�
    _float4                         qtOrientation;                  // ����, ���߿� ���ʹϿ����� �ٲ� ��
    _float3                         vScale;                         // ũ��
    

    _float3x3                       matInverseInertiaTensorWorld;   // 3���� ���
    _float3x4                       matTransform;                   // 4���� ���, Ʈ������
    
    _float                          fMotion;                        // ���
    _bool                           bIsAwake;                       // �� �ۿ� ������Ʈ ���� ����
    _bool                           bCanSleep;                      // �۽Ƿ� �ڵ� ���� ���� ���

public:
    GETSET_2(void*, pOwner, Owner, GET, SET)

protected:
    void*           pOwner = nullptr;                // �浹������ ���, 
};

END