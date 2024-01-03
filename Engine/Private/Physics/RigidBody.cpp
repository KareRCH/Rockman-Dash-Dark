#include "Physics/RigidBody.h"

#include "Physics/CollisionPrimitive.h"



void FRigidBody::CalculateDerivedData()
{
	_vector vSimPosition = XMLoadFloat3(&vPosition);
	_vector vSimOrientation = XMLoadFloat4(&qtOrientation);
	_vector vSimScale = XMLoadFloat3(&vScale);
	_matrix matSimTransform = XMLoadFloat3x4(&matTransform);
	// 회전 쿼터니언 정규화
	XMStoreFloat4(&qtOrientation, XMQuaternionNormalize(vSimOrientation));

	// 몸체에 대한 트랜스폼 계산
	matSimTransform = XMMatrixAffineTransformation(vSimScale, XMQuaternionIdentity(), vSimOrientation, vSimPosition);
	XMStoreFloat3x4(&matTransform, matSimTransform);

	// 월드 스페이스에서 관성 텐서를 계산
	/*_TransformInertiaTensor(matInverseInertiaTensorWorld, qtOrientation,
								matInverseInertiaTensor, matTransform);*/
}

void FRigidBody::Integrate(const _float& fDuration)
{
	if (eBodyType == ERIGID_BODY_TYPE::STATIC) return;					// STATIC이면 힘을 부과하지 않는다.
	if (eBodyType == ERIGID_BODY_TYPE::DYNAMIC && !bIsAwake) return;	// DYNAMIC에 대해서만 재운다.

	// 선형 가속 합을 가속에 더하기
	vLastFrameAcceleration = vAcceleration;
	_vector vSimLastFramAcceleration = XMLoadFloat3(&vLastFrameAcceleration);
	vSimLastFramAcceleration += XMLoadFloat3(&vForceAccum) * fInverseMass;
	XMStoreFloat3(&vLastFrameAcceleration, vSimLastFramAcceleration);

	// 각 가속도를 계산
	_matrix matSimInverseInertiaTensorWorld = XMLoadFloat3x3(&matInverseInertiaTensorWorld);
	_vector vSimTorqueAccum = XMLoadFloat3(&vTorqueAccum);
	_vector vAngularAcceleration = XMVector3TransformCoord(vSimTorqueAccum, matSimInverseInertiaTensorWorld);

	// 속도 조절
	// 선형 속도를 가속도와 충격으로부터 업데이트 한다.
	_vector vSimVelocity = XMLoadFloat3(&vVelocity);
	vSimVelocity += vSimLastFramAcceleration * fDuration;
	
	// 각 속도를 가속도와 충격으로부터 업데이트 한다.
	_vector vSimRotation = XMLoadFloat3(&vRotation);
	vSimRotation += vAngularAcceleration * fDuration;
	
	// 드래그 값을 부과한다.
	vSimVelocity *= pow(fLinearDamping, fDuration);
	vSimRotation *= pow(fAngularDamping, fDuration);

	XMStoreFloat3(&vVelocity, vSimVelocity);
	XMStoreFloat3(&vRotation, vSimRotation);

	// 위치를 조절한다.
	// 선형 위치를 업데이트 한다.
	_vector vSimPosition = XMLoadFloat3(&vPosition);
	vSimPosition += vSimVelocity * fDuration;
	XMStoreFloat3(&vPosition, vSimPosition);

	// 각 위치를 업데이트 한다.
	_vector vSimOrientation = XMLoadFloat4(&qtOrientation);
	vSimOrientation = XMQuaternionRotationRollPitchYawFromVector(vSimRotation) * fDuration;

	// 정위를 정규화, 새로운 행렬을 업데이트한다.
	// 위치, 정위
	CalculateDerivedData();

	// 힘 합산기들을 정리한다.
	Clear_Accumulators();

	// 운동 에너지 저장을 업데이트, 몸체를 재울 수 있으면 재운다.
	if (eBodyType == ERIGID_BODY_TYPE::DYNAMIC && bCanSleep)
	{
		// 각 벡터들을 제곱한 값
		_float fCurrentMotion = XMVectorGetX(XMVector3Dot(vSimVelocity, vSimVelocity) + XMVector3Dot(vSimRotation, vSimRotation));

		// 움직이고 있는지에 대한 평가치를 계산
		_float fBias = pow(0.5f, fDuration);
		fMotion = (fBias * fMotion) + ((1 - fBias) * fCurrentMotion);

		// 오차와 계산해서 깨울지 재울지 결정
		//if (fMotion < fSleepEpsilon) { Set_Awake(false); }
		//else if (fMotion > 10 * fSleepEpsilon) { fMotion = 10 * fSleepEpsilon; }
	}
}
