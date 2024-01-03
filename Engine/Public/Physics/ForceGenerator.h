#pragma once

#include "Engine_Define.h"

#include "Physics/RigidBody.h"

BEGIN(Engine)

class FForceGenerator;

/// <summary>
/// �� ��ϱ�
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
/// ���� ���� �߻���Ű�� Ŭ������ ����
/// </summary>
class ENGINE_DLL FForceGenerator abstract
{
	THIS_CLASS(FForceGenerator)

public:
	virtual void Update_Force(FRigidBody* pBody, const _float& fDuration) PURE;

};


/// <summary>
/// �߷� �� �߻���
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
/// ������ �� �߻���
/// </summary>
class ENGINE_DLL FForce_Spring
{
	THIS_CLASS(FForce_Spring)

private:
	_float3 vConnectionPoint;		// ���� ����Ʈ
	_float3 vOtherConnectionPoint;	// �ٸ� ��ü ���� ����Ʈ

	FRigidBody* pOther;				// �ٸ� ��ü

	_float		fSpringConstant;	// ������ ���
	_float		fRestLength;		// ���� ����, ���� ����

public:
	FForce_Spring(const _float3& localConnectionPt, FRigidBody* other, const _float3& otherConnectionPt,
						_float springConstant, _float restLength);

	virtual void Update_Force(FRigidBody* pBody, const _float& fDuration);
};

END