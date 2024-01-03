#include "Physics/RigidBody.h"

#include "Physics/CollisionPrimitive.h"



void FRigidBody::CalculateDerivedData()
{
	_vector vSimPosition = XMLoadFloat3(&vPosition);
	_vector vSimOrientation = XMLoadFloat4(&qtOrientation);
	_vector vSimScale = XMLoadFloat3(&vScale);
	_matrix matSimTransform = XMLoadFloat3x4(&matTransform);
	// ȸ�� ���ʹϾ� ����ȭ
	XMStoreFloat4(&qtOrientation, XMQuaternionNormalize(vSimOrientation));

	// ��ü�� ���� Ʈ������ ���
	matSimTransform = XMMatrixAffineTransformation(vSimScale, XMQuaternionIdentity(), vSimOrientation, vSimPosition);
	XMStoreFloat3x4(&matTransform, matSimTransform);

	// ���� �����̽����� ���� �ټ��� ���
	/*_TransformInertiaTensor(matInverseInertiaTensorWorld, qtOrientation,
								matInverseInertiaTensor, matTransform);*/
}

void FRigidBody::Integrate(const _float& fDuration)
{
	if (eBodyType == ERIGID_BODY_TYPE::STATIC) return;					// STATIC�̸� ���� �ΰ����� �ʴ´�.
	if (eBodyType == ERIGID_BODY_TYPE::DYNAMIC && !bIsAwake) return;	// DYNAMIC�� ���ؼ��� ����.

	// ���� ���� ���� ���ӿ� ���ϱ�
	vLastFrameAcceleration = vAcceleration;
	_vector vSimLastFramAcceleration = XMLoadFloat3(&vLastFrameAcceleration);
	vSimLastFramAcceleration += XMLoadFloat3(&vForceAccum) * fInverseMass;
	XMStoreFloat3(&vLastFrameAcceleration, vSimLastFramAcceleration);

	// �� ���ӵ��� ���
	_matrix matSimInverseInertiaTensorWorld = XMLoadFloat3x3(&matInverseInertiaTensorWorld);
	_vector vSimTorqueAccum = XMLoadFloat3(&vTorqueAccum);
	_vector vAngularAcceleration = XMVector3TransformCoord(vSimTorqueAccum, matSimInverseInertiaTensorWorld);

	// �ӵ� ����
	// ���� �ӵ��� ���ӵ��� ������κ��� ������Ʈ �Ѵ�.
	_vector vSimVelocity = XMLoadFloat3(&vVelocity);
	vSimVelocity += vSimLastFramAcceleration * fDuration;
	
	// �� �ӵ��� ���ӵ��� ������κ��� ������Ʈ �Ѵ�.
	_vector vSimRotation = XMLoadFloat3(&vRotation);
	vSimRotation += vAngularAcceleration * fDuration;
	
	// �巡�� ���� �ΰ��Ѵ�.
	vSimVelocity *= pow(fLinearDamping, fDuration);
	vSimRotation *= pow(fAngularDamping, fDuration);

	XMStoreFloat3(&vVelocity, vSimVelocity);
	XMStoreFloat3(&vRotation, vSimRotation);

	// ��ġ�� �����Ѵ�.
	// ���� ��ġ�� ������Ʈ �Ѵ�.
	_vector vSimPosition = XMLoadFloat3(&vPosition);
	vSimPosition += vSimVelocity * fDuration;
	XMStoreFloat3(&vPosition, vSimPosition);

	// �� ��ġ�� ������Ʈ �Ѵ�.
	_vector vSimOrientation = XMLoadFloat4(&qtOrientation);
	vSimOrientation = XMQuaternionRotationRollPitchYawFromVector(vSimRotation) * fDuration;

	// ������ ����ȭ, ���ο� ����� ������Ʈ�Ѵ�.
	// ��ġ, ����
	CalculateDerivedData();

	// �� �ջ����� �����Ѵ�.
	Clear_Accumulators();

	// � ������ ������ ������Ʈ, ��ü�� ��� �� ������ ����.
	if (eBodyType == ERIGID_BODY_TYPE::DYNAMIC && bCanSleep)
	{
		// �� ���͵��� ������ ��
		_float fCurrentMotion = XMVectorGetX(XMVector3Dot(vSimVelocity, vSimVelocity) + XMVector3Dot(vSimRotation, vSimRotation));

		// �����̰� �ִ����� ���� ��ġ�� ���
		_float fBias = pow(0.5f, fDuration);
		fMotion = (fBias * fMotion) + ((1 - fBias) * fCurrentMotion);

		// ������ ����ؼ� ������ ����� ����
		//if (fMotion < fSleepEpsilon) { Set_Awake(false); }
		//else if (fMotion > 10 * fSleepEpsilon) { fMotion = 10 * fSleepEpsilon; }
	}
}
