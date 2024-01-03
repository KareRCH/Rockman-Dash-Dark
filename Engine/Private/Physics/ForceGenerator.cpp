#include "Physics/ForceGenerator.h"


#pragma region �� ��ϱ�

void FForceRegistry::Update_Forces(const _float fDuration)
{
    auto iter = vecRegistrations.begin();
    for (; iter != vecRegistrations.end(); iter++)
    {
        // ��ϱ⿡ ��ϵ� ��� ��ü�� ���� ����� ���� ���� ������Ʈ �Ѵ�.
        iter->pFGen->Update_Force(iter->pBody, fDuration);
    }
}

void FForceRegistry::Add(FRigidBody* pBody, FForceGenerator* pFGen)
{
    FForceRegistry::FForceRegistration registration;
    registration.pBody = pBody;
    registration.pFGen = pFGen;
    vecRegistrations.push_back(registration);
}

#pragma endregion



#pragma region �߷�

FForce_Gravity::FForce_Gravity(const _float3& vGravity)
    : vGravity(vGravity)
{
}

void FForce_Gravity::Update_Force(FRigidBody* pBody, const _float& fDuration)
{
    // Check that we do not have infinite mass
    if (!pBody->HasFiniteMass()) return;

    // Apply the mass-scaled force to the body
    _float3 vGivGravity = {};
    _vector vSimGravity = XMLoadFloat3(&vGravity);
    XMStoreFloat3(&vGivGravity, vSimGravity * pBody->Get_Mass());
    pBody->Add_Force(vGivGravity);
}

#pragma endregion






#pragma region ������

FForce_Spring::FForce_Spring(const _float3& localConnectionPt, FRigidBody* other, const _float3& otherConnectionPt,
                                _float springConstant, _float restLength)
    : vConnectionPoint(localConnectionPt), vOtherConnectionPoint(otherConnectionPt), pOther(other),
    fSpringConstant(springConstant), fRestLength(restLength)
{
}

void FForce_Spring::Update_Force(FRigidBody* pBody, const _float& fDuration)
{
    //// Calculate the two ends in world space
    //_float3 lws = pBody->Get_PointInWorldSpace(vConnectionPoint);
    //_float3 ows = pBody->Get_PointInWorldSpace(vOtherConnectionPoint);

    //// Calculate the vector of the spring
    //_float3 force = lws - ows;

    //// Calculate the magnitude of the force
    //_float magnitude = force.Magnitude();
    //magnitude = real_abs(magnitude - fRestLength);
    //magnitude *= fSpringConstant;

    //// Calculate the final force and apply it
    //force.Normalize();
    //force *= -magnitude;
    //pBody->Add_ForceAtPoint(force, lws);
}

#pragma endregion
