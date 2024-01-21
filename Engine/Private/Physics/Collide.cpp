#include "Physics/Collide.h"


bool FCollisionDetector::CollsionPrimitive(const FCollisionPrimitive* srcShape, const FCollisionPrimitive* dstShape, FCollisionData* pColData)
{
	bool bCollide = false;
	switch (srcShape->Get_Type())
	{
	case ECollisionType::Sphere:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::SphereAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::SphereAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::SphereAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::SphereAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::SphereAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::SphereAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::SphereAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionSphere* pShapeSrc = static_cast<const FCollisionSphere*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::SphereAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	case ECollisionType::Box:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::BoxAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::BoxAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::BoxAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::BoxAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::BoxAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::BoxAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::BoxAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionBox* pShapeSrc = static_cast<const FCollisionBox*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::BoxAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	case ECollisionType::Capsule:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionCapsule* pShapeSrc = static_cast<const FCollisionCapsule*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::CapsuleAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	case ECollisionType::OBB:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::OBBAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::OBBAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::OBBAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::OBBAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::OBBAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::OBBAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::OBBAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionOBB* pShapeSrc = static_cast<const FCollisionOBB*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::OBBAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	case ECollisionType::Line:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::LineAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::LineAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::LineAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::LineAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::LineAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::LineAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::LineAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionLine* pShapeSrc = static_cast<const FCollisionLine*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::LineAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	case ECollisionType::Ray:
	{
		switch (dstShape->Get_Type())
		{
		case ECollisionType::Sphere:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionSphere* pShapeDst = static_cast<const FCollisionSphere*>(dstShape);
			bCollide = FCollisionDetector::RayAndSphere(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Box:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionBox* pShapeDst = static_cast<const FCollisionBox*>(dstShape);
			bCollide = FCollisionDetector::RayAndBox(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Capsule:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionCapsule* pShapeDst = static_cast<const FCollisionCapsule*>(dstShape);
			bCollide = FCollisionDetector::RayAndCapsule(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Plane:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionPlane* pShapeDst = static_cast<const FCollisionPlane*>(dstShape);
			bCollide = FCollisionDetector::RayAndPlane(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Line:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionLine* pShapeDst = static_cast<const FCollisionLine*>(dstShape);
			bCollide = FCollisionDetector::RayAndLine(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Ray:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionRay* pShapeDst = static_cast<const FCollisionRay*>(dstShape);
			bCollide = FCollisionDetector::RayAndRay(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::Triangle:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionTriangle* pShapeDst = static_cast<const FCollisionTriangle*>(dstShape);
			bCollide = FCollisionDetector::RayAndTriangle(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		case ECollisionType::OBB:
		{
			const FCollisionRay* pShapeSrc = static_cast<const FCollisionRay*>(srcShape);
			const FCollisionOBB* pShapeDst = static_cast<const FCollisionOBB*>(dstShape);
			bCollide = FCollisionDetector::RayAndOBB(*pShapeSrc, *pShapeDst, pColData);
			break;
		}
		}
		break;
	}
	}

	return bCollide;
}

bool FCollisionDetector::SphereAndSphere(const FCollisionSphere& srcSphere, const FCollisionSphere& dstSphere, FCollisionData* pColData)
{
	_vector vSimSub = dstSphere.Get_PositionVector() - srcSphere.Get_PositionVector();
	_float fPenetration = srcSphere.fRadius + dstSphere.fRadius - XMVectorGetX(XMVector3Length(vSimSub));

	// 충돌정보 생성
	if (pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		XMStoreFloat3(&pContact.vContactNormal, vSimSub * (1.0f / XMVectorGetX(XMVector3Length(vSimSub))));
		XMStoreFloat3(&pContact.vContactPoint, srcSphere.Get_PositionVector() + vSimSub * 0.5f);
		pContact.fPenetration = fPenetration;
		pContact.Set_BodyData(srcSphere.pBody, dstSphere.pBody, pColData->fFriction, pColData->fRestitution);
	}

	return fPenetration >= 0;
}

bool FCollisionDetector::SphereAndBox(const FCollisionSphere& srcSphere, const FCollisionBox& dstBox, FCollisionData* pColData)
{
	_vector vSimResult = srcSphere.Get_PositionVector();
	_vector vSimMin = dstBox.Get_PositionVector() - XMLoadFloat3(&dstBox.vHalfSize);
	_vector vSimMax = dstBox.Get_PositionVector() + XMLoadFloat3(&dstBox.vHalfSize);

	XMVectorSetX(vSimResult, (XMVectorGetX(vSimResult) < XMVectorGetX(vSimMin)) ? XMVectorGetX(vSimMin) : XMVectorGetX(vSimResult));
	XMVectorSetY(vSimResult, (XMVectorGetY(vSimResult) < XMVectorGetY(vSimMin)) ? XMVectorGetY(vSimMin) : XMVectorGetY(vSimResult));
	XMVectorSetY(vSimResult, (XMVectorGetZ(vSimResult) < XMVectorGetZ(vSimMin)) ? XMVectorGetZ(vSimMin) : XMVectorGetZ(vSimResult));

	XMVectorSetX(vSimResult, (XMVectorGetX(vSimResult) > XMVectorGetX(vSimMax)) ? XMVectorGetX(vSimMax) : XMVectorGetX(vSimResult));
	XMVectorSetY(vSimResult, (XMVectorGetY(vSimResult) > XMVectorGetY(vSimMax)) ? XMVectorGetY(vSimMax) : XMVectorGetY(vSimResult));
	XMVectorSetY(vSimResult, (XMVectorGetZ(vSimResult) > XMVectorGetZ(vSimMax)) ? XMVectorGetZ(vSimMax) : XMVectorGetZ(vSimResult));

	_vector vClosestPoint = vSimResult;
	_float fDist = XMVectorGetX(XMVector3Length(srcSphere.Get_PositionVector() - vClosestPoint));
	_float fDistSq = fDist * fDist;
	_float fRadiusSq = (srcSphere.fRadius * srcSphere.fRadius);

	// 충돌정보 생성
	if (pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		XMStoreFloat3(&pContact.vContactNormal, XMVector3Normalize(vClosestPoint - srcSphere.Get_PositionVector()));
		XMStoreFloat3(&pContact.vContactPoint, vClosestPoint);
		pContact.fPenetration = srcSphere.fRadius - fDist;
		pContact.Set_BodyData(srcSphere.pBody, dstBox.pBody, pColData->fFriction, pColData->fRestitution);
	}

	return fDistSq < fRadiusSq;
}

bool FCollisionDetector::SphereAndCapsule(const FCollisionSphere& srcSphere, const FCollisionCapsule& dstCapsule, FCollisionData* pColData)
{
	_vector vDst_HalfSize = XMLoadFloat3(&dstCapsule.vDirHalfSize);
	_vector vDst_Normal = XMVector3Normalize(vDst_HalfSize);;
	_vector vDst_LineEndOffset = vDst_Normal * dstCapsule.fRadius;
	_vector vDst_A = dstCapsule.Get_PositionVector() - vDst_HalfSize + vDst_LineEndOffset;			// A 구 위치
	_vector vDst_B = dstCapsule.Get_PositionVector() + vDst_HalfSize - vDst_LineEndOffset;			// B 구 위치

	_vector vBest = FLineTests::ClosestPointOnLineSegment(vDst_A, vDst_B, srcSphere.Get_PositionVector());

	_vector vSub = vBest - srcSphere.Get_PositionVector();
	_float fSqLength = XMVectorGetX(XMVector3LengthSq(vSub));

	return ((srcSphere.fRadius + dstCapsule.fRadius) * (srcSphere.fRadius + dstCapsule.fRadius) >= fSqLength);
}

bool FCollisionDetector::SphereAndPlane(const FCollisionSphere& srcSphere, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{

	return false;
}

bool FCollisionDetector::SphereAndTriangle(const FCollisionSphere& srcSphere, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::SphereAndOBB(const FCollisionSphere& srcSphere, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	// 
	_vector vResult = dstOBB.Get_PositionVector();
	_vector vDir = srcSphere.Get_PositionVector() - dstOBB.Get_PositionVector();

	_matrix matTransform = XMLoadFloat3x4(&dstOBB.Get_Transform());
	for (int i = 0; i < 3; i++)
	{
		_vector vAxis = XMVector3Normalize(matTransform.r[i]);

		_float fDistance = XMVectorGetX(XMVector3Dot(vAxis, vDir));

		if (i == 0)
		{
			if (fDistance > dstOBB.vHalfSize.x)
				fDistance = dstOBB.vHalfSize.x;
			else if (fDistance < -dstOBB.vHalfSize.x)
				fDistance = -dstOBB.vHalfSize.x;
		}
		else if (i == 1)
		{
			if (fDistance > dstOBB.vHalfSize.y)
				fDistance = dstOBB.vHalfSize.y;
			else if (fDistance < -dstOBB.vHalfSize.y)
				fDistance = -dstOBB.vHalfSize.y;
		}
		else if (i == 2)
		{
			if (fDistance > dstOBB.vHalfSize.z)
				fDistance = dstOBB.vHalfSize.z;
			else if (fDistance < -dstOBB.vHalfSize.z)
				fDistance = -dstOBB.vHalfSize.z;
		}

		vResult += (vAxis * fDistance);
	}


	// 구한 접점을 가지고 거리 체크
	_vector& vClosestPoint = vResult;
	_float fDistSq_Between = XMVectorGetX(XMVector3LengthSq(vDir));
	_float fDistSq_ClosestFromSphere = XMVectorGetX(XMVector3LengthSq(srcSphere.Get_PositionVector() - vClosestPoint));
	_float fDistSq_ClosestFromOBB = XMVectorGetX(XMVector3LengthSq(dstOBB.Get_PositionVector() - vClosestPoint));
	_float fRadiusSq = (srcSphere.fRadius * srcSphere.fRadius);
	_bool bCollide = (fDistSq_ClosestFromSphere < fRadiusSq) || (fDistSq_Between < fDistSq_ClosestFromOBB);

	/*FVector3 vClosestPoint = dstOBB.Get_Transform().Transform(vResult);
	Real fDistSq_Between = vDir.SquareMagnitude();
	Real fDistSq_ClosestFromSphere = (srcSphere.Get_Position() - vClosestPoint).SquareMagnitude();
	Real fDistSq_ClosestFromOBB = (dstOBB.Get_Position() - vClosestPoint).SquareMagnitude();
	Real fRadiusSq = (srcSphere.fRadius * srcSphere.fRadius);
	_bool bCollide = (fDistSq_ClosestFromSphere < fRadiusSq) || (fDistSq_Between < fDistSq_ClosestFromOBB);*/

	// 충돌정보 생성
	if (bCollide
	&& pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		
		XMStoreFloat3(&pContact.vContactNormal, XMVector3Normalize(vClosestPoint - srcSphere.Get_PositionVector()));
		// 구체의 중점이 OBB에 파묻힘, 노멀을 반대로
		if (fDistSq_Between < fDistSq_ClosestFromOBB)
		{
			pContact.vContactNormal.x = -pContact.vContactNormal.x;
			pContact.vContactNormal.y = -pContact.vContactNormal.y;
			pContact.vContactNormal.z = -pContact.vContactNormal.z;
		}
			
		XMStoreFloat3(&pContact.vContactPoint, vClosestPoint);
		if (fDistSq_ClosestFromSphere < fRadiusSq)
			pContact.fPenetration = srcSphere.fRadius - sqrt(fDistSq_ClosestFromSphere);
		else if (fDistSq_Between < fDistSq_ClosestFromOBB)
			pContact.fPenetration = srcSphere.fRadius + sqrt(fDistSq_ClosestFromOBB);
		pContact.Set_BodyData(srcSphere.pBody, dstOBB.pBody, pColData->fFriction, pColData->fRestitution);
	}
	
	return bCollide;
}

bool FCollisionDetector::BoxAndBox(const FCollisionBox& srcBox, const FCollisionBox& dstBox, FCollisionData* pColData)
{
	_vector vSrc_Pos = srcBox.Get_PositionVector();
	_vector vSrc_HalfSize = XMLoadFloat3(&srcBox.vHalfSize);
	_vector vDst_Pos = dstBox.Get_PositionVector();
	_vector vDst_HalfSize = XMLoadFloat3(&dstBox.vHalfSize);
	
	_vector vBool1 = XMVectorGreaterOrEqual(vSrc_Pos + vSrc_HalfSize, vDst_Pos - vDst_HalfSize);
	_vector vBool2 = XMVectorGreaterOrEqual(vDst_Pos + vDst_HalfSize, vSrc_Pos - vSrc_HalfSize);

	return XMVectorGetX(vBool1) && XMVectorGetY(vBool1) && XMVectorGetZ(vBool1)
		&& XMVectorGetX(vBool2) && XMVectorGetY(vBool2) && XMVectorGetZ(vBool2);
}

bool FCollisionDetector::BoxAndCapsule(const FCollisionBox& srcBox, const FCollisionCapsule& dstCapsule, FCollisionData* pColData)
{
	// 최적의 구 위치 선별해내기
	_vector vDst_Pos = dstCapsule.Get_PositionVector();
	_vector vDst_DirHalfSize = XMLoadFloat3(&dstCapsule.vDirHalfSize);
	_vector vDst_Normal = XMVector3Normalize(vDst_DirHalfSize);
	_vector vDst_LineEndOffset = vDst_Normal * dstCapsule.fRadius;
	_vector vDst_A = vDst_Pos - vDst_DirHalfSize + vDst_LineEndOffset;			// A 구 위치
	_vector vDst_B = vDst_Pos + vDst_DirHalfSize - vDst_LineEndOffset;			// B 구 위치

	_vector vBox_Pos = srcBox.Get_PositionVector();
	_vector vBox_HalfSize = XMLoadFloat3(&srcBox.vHalfSize);

	_vector vBest = FLineTests::ClosestPointOnLineSegment(vDst_A, vDst_B, vBox_Pos);

	// 박스 구 충돌
	_vector vResult = vBest;
	_vector vMin = vBox_Pos - vBox_HalfSize;
	_vector vMax = vBox_Pos + vBox_HalfSize;

	XMVectorSetX(vResult, (XMVectorGetX(vResult) < XMVectorGetX(vMin)) ? XMVectorGetX(vMin) : XMVectorGetX(vResult));
	XMVectorSetY(vResult, (XMVectorGetY(vResult) < XMVectorGetY(vMin)) ? XMVectorGetY(vMin) : XMVectorGetY(vResult));
	XMVectorSetY(vResult, (XMVectorGetZ(vResult) < XMVectorGetZ(vMin)) ? XMVectorGetZ(vMin) : XMVectorGetZ(vResult));

	XMVectorSetX(vResult, (XMVectorGetX(vResult) > XMVectorGetX(vMax)) ? XMVectorGetX(vMax) : XMVectorGetX(vResult));
	XMVectorSetY(vResult, (XMVectorGetY(vResult) > XMVectorGetY(vMax)) ? XMVectorGetY(vMax) : XMVectorGetY(vResult));
	XMVectorSetY(vResult, (XMVectorGetZ(vResult) > XMVectorGetZ(vMax)) ? XMVectorGetZ(vMax) : XMVectorGetZ(vResult));

	_vector& vClosestPoint = vResult;
	Real fDistSq = XMVectorGetX(XMVector3LengthSq(vBest - vClosestPoint));
	Real fRadiusSq = (dstCapsule.fRadius * dstCapsule.fRadius);

	return fDistSq < fRadiusSq;
}

bool FCollisionDetector::BoxAndPlane(const FCollisionBox& srcBox, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::BoxAndTriangle(const FCollisionBox& srcBox, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::BoxAndOBB(const FCollisionBox& srcBox, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::CapsuleAndCapsule(const FCollisionCapsule& srcCapsule, const FCollisionCapsule& dstCapsule, FCollisionData* pColData)
{
	_vector vSrc_Pos = srcCapsule.Get_PositionVector();
	_vector vSrc_DirHalfSize = XMLoadFloat3(&srcCapsule.vDirHalfSize);
	_vector vSrc_Normal = XMVector3Normalize(vSrc_DirHalfSize);
	_vector vSrc_LineEndOffset = vSrc_Normal * srcCapsule.fRadius;
	_vector vSrc_A = vSrc_Pos - vSrc_DirHalfSize + vSrc_LineEndOffset;			// A 구 위치
	_vector vSrc_B = vSrc_Pos + vSrc_DirHalfSize - vSrc_LineEndOffset;			// B 구 위치

	_vector vDst_Pos = dstCapsule.Get_PositionVector();
	_vector vDst_DirHalfSize = XMLoadFloat3(&dstCapsule.vDirHalfSize);
	_vector vDst_Normal = XMVector3Normalize(vDst_DirHalfSize);
	_vector vDst_LineEndOffset = vDst_Normal * dstCapsule.fRadius;
	_vector vDst_A = vDst_Pos - vDst_DirHalfSize + vDst_LineEndOffset;			// A 구 위치
	_vector vDst_B = vDst_Pos + vDst_DirHalfSize - vDst_LineEndOffset;			// B 구 위치

	_vector vAA = vDst_A - vSrc_A;
	_vector vAB = vDst_A - vSrc_B;
	_vector vBA = vDst_B - vSrc_A;
	_vector vBB = vDst_B - vSrc_B;

	_float fAA = XMVectorGetX(XMVector3LengthSq(vAA));
	_float fAB = XMVectorGetX(XMVector3LengthSq(vAB));
	_float fBA = XMVectorGetX(XMVector3LengthSq(vBA));
	_float fBB = XMVectorGetX(XMVector3LengthSq(vBB));


	// A와 B중에 가장 가까운 구체의 점을 선택한다.
	_vector vBestA;
	if (fBA < fAA || fBA < fAB || fBB < fAA || fBB < fAB)
		vBestA = vSrc_B;
	else
		vBestA = vSrc_A;

	// 캡슐 Dst의 두점과 Src의 최적의 점을 가지고 계산
	_vector vBestB = FLineTests::ClosestPointOnLineSegment(vDst_A, vDst_B, vBestA);

	// 반대의 경우도 계산
	vBestA = FLineTests::ClosestPointOnLineSegment(vSrc_A, vSrc_B, vBestB);

	_vector vPenetration_Normal = vBestB - vBestA;
	_float fLength = XMVectorGetX(XMVector3Length(vPenetration_Normal));
	_float fPenetration_Depth = srcCapsule.fRadius + dstCapsule.fRadius - fLength;
	bool bIntersects = fPenetration_Depth > 0.f;

	// 충돌정보 생성
	if (bIntersects
	&& pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		XMStoreFloat3(&pContact.vContactNormal, XMVector3Normalize(vPenetration_Normal));
		XMStoreFloat3(&pContact.vContactPoint, vBestA + vPenetration_Normal * 0.5f);
		pContact.fPenetration = fPenetration_Depth;
		pContact.Set_BodyData(srcCapsule.pBody, dstCapsule.pBody, pColData->fFriction, pColData->fRestitution);
	}

	return bIntersects;
}

bool FCollisionDetector::CapsuleAndPlane(const FCollisionCapsule& srcCapsule, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::CapsuleAndTriangle(const FCollisionCapsule& srcCapsule, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::CapsuleAndOBB(const FCollisionCapsule& srcCapsule, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	_vector vSrc_Pos = srcCapsule.Get_PositionVector();
	_vector vSrc_HalfSize = XMLoadFloat3(&srcCapsule.vDirHalfSize);
	_vector vSrc_Normal = XMVector3Normalize(vSrc_HalfSize);
	_vector vSrc_LineEndOffset = vSrc_Normal * srcCapsule.fRadius;
	_vector vSrc_A = vSrc_Pos - vSrc_HalfSize + vSrc_LineEndOffset;			// A 구 위치
	_vector vSrc_B = vSrc_Pos + vSrc_HalfSize - vSrc_LineEndOffset;			// B 구 위치

	_vector vResult = dstOBB.Get_PositionVector();

	_vector vSphere = FLineTests::ClosestPointOnLineSegment(vSrc_A, vSrc_B, vResult);

	_vector vDir = vSphere - dstOBB.Get_PositionVector();

	_matrix matTransform = XMLoadFloat3x4(&dstOBB.Get_Transform());
	for (int i = 0; i < 3; i++)
	{
		_vector vAxis = XMVector3Normalize(matTransform.r[i]);

		_float fDistance = XMVectorGetX(XMVector3Dot(vAxis, vDir));

		if (i == 0)
		{
			if (fDistance > dstOBB.vHalfSize.x)
				fDistance = dstOBB.vHalfSize.x;
			else if (fDistance < -dstOBB.vHalfSize.x)
				fDistance = -dstOBB.vHalfSize.x;
		}
		else if (i == 1)
		{
			if (fDistance > dstOBB.vHalfSize.y)
				fDistance = dstOBB.vHalfSize.y;
			else if (fDistance < -dstOBB.vHalfSize.y)
				fDistance = -dstOBB.vHalfSize.y;
		}
		else if (i == 2)
		{
			if (fDistance > dstOBB.vHalfSize.z)
				fDistance = dstOBB.vHalfSize.z;
			else if (fDistance < -dstOBB.vHalfSize.z)
				fDistance = -dstOBB.vHalfSize.z;
		}
		

		vResult += (vAxis * fDistance);
	}

	// 구한 접점을 가지고 거리 체크
	_vector& vClosestPoint = vResult;
	_float fDistSq_Between = XMVectorGetX(XMVector3LengthSq(vDir));
	_float fDistSq_ClosestFromSphere = XMVectorGetX(XMVector3LengthSq(vSphere - vClosestPoint));
	_float fDistSq_ClosestFromOBB = XMVectorGetX(XMVector3LengthSq(dstOBB.Get_PositionVector() - vClosestPoint));
	_float fRadiusSq = (srcCapsule.fRadius * srcCapsule.fRadius);
	_bool bCollide = (fDistSq_ClosestFromSphere < fRadiusSq) || (fDistSq_Between < fDistSq_ClosestFromOBB);


	// 충돌정보 생성
	if (bCollide
		&& pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;

		XMStoreFloat3(&pContact.vContactNormal, XMVector3Normalize(vClosestPoint - vSphere));
		// 구체의 중점이 OBB에 파묻힘, 노멀을 반대로
		if (fDistSq_Between < fDistSq_ClosestFromOBB)
		{
			pContact.vContactNormal.x = -pContact.vContactNormal.x;
			pContact.vContactNormal.y = -pContact.vContactNormal.y;
			pContact.vContactNormal.z = -pContact.vContactNormal.z;
		}

		XMStoreFloat3(&pContact.vContactPoint, vClosestPoint);
		if (fDistSq_ClosestFromSphere < fRadiusSq)
			pContact.fPenetration = srcCapsule.fRadius - real_sqrt(fDistSq_ClosestFromSphere);
		else if (fDistSq_Between < fDistSq_ClosestFromOBB)
			pContact.fPenetration = srcCapsule.fRadius + real_sqrt(fDistSq_ClosestFromOBB);
		pContact.Set_BodyData(srcCapsule.pBody, dstOBB.pBody, pColData->fFriction, pColData->fRestitution);
	}

	return bCollide;
}

bool FCollisionDetector::PlaneAndPlane(const FCollisionPlane& srcPlane, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndCapsule(const FCollisionLine& srcLine, const FCollisionCapsule& dstCapsule, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndBox(const FCollisionLine& srcLine, const FCollisionBox& dstBox, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndSphere(const FCollisionLine& srcLine, const FCollisionSphere& dstSphere, FCollisionData* pColData)
{
	_vector vLineStart = XMLoadFloat3(&srcLine.vStart);
	_vector vLineEnd = XMLoadFloat3(&srcLine.vEnd);
	_vector vSpherePos = dstSphere.Get_PositionVector();

	_vector vBest = FLineTests::ClosestPointOnLineSegment(vLineStart, vLineEnd, vSpherePos);

	_vector vSub = vBest - vSpherePos;
	Real fSqLength = XMVectorGetX(XMVector3LengthSq(vSub));

	return ((dstSphere.fRadius * dstSphere.fRadius) >= fSqLength);
}

bool FCollisionDetector::LineAndPlane(const FCollisionLine& srcLine, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndLine(const FCollisionLine& srcLine, const FCollisionLine& dstLine, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndRay(const FCollisionLine& srcLine, const FCollisionRay& dstRay, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndTriangle(const FCollisionLine& srcLine, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::LineAndOBB(const FCollisionLine& srcLine, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::RayAndCapsule(const FCollisionRay& srcRay, const FCollisionCapsule& dstCapsule, FCollisionData* pColData)
{
	

	return false;
}

bool FCollisionDetector::RayAndBox(const FCollisionRay& srcRay, const FCollisionBox& dstBox, FCollisionData* pColData)
{
	_vector vMin = dstBox.Get_PositionVector() - XMLoadFloat3(&dstBox.vHalfSize);
	_vector vMax = dstBox.Get_PositionVector() + XMLoadFloat3(&dstBox.vHalfSize);

	// Any component of direction could be 0!
	// Address this by using a small number, close to
	// 0 in case any of directions components are 0
	_float fT1 = (XMVectorGetX(vMin) - srcRay.vOrigin.x) / ((CMP(srcRay.vDir.x, 0.0f) ? 0.00001f : srcRay.vDir.x));
	_float fT2 = (XMVectorGetX(vMax) - srcRay.vOrigin.x) / ((CMP(srcRay.vDir.x, 0.0f) ? 0.00001f : srcRay.vDir.x));
	_float fT3 = (XMVectorGetY(vMin) - srcRay.vOrigin.y) / ((CMP(srcRay.vDir.y, 0.0f) ? 0.00001f : srcRay.vDir.y));
	_float fT4 = (XMVectorGetY(vMax) - srcRay.vOrigin.y) / ((CMP(srcRay.vDir.y, 0.0f) ? 0.00001f : srcRay.vDir.y));
	_float fT5 = (XMVectorGetZ(vMin) - srcRay.vOrigin.z) / ((CMP(srcRay.vDir.z, 0.0f) ? 0.00001f : srcRay.vDir.z));
	_float fT6 = (XMVectorGetZ(vMax) - srcRay.vOrigin.z) / ((CMP(srcRay.vDir.z, 0.0f) ? 0.00001f : srcRay.vDir.z));

	_float fTmin = max(max(min(fT1, fT2), min(fT3, fT4)), min(fT5, fT6));
	_float fTMax = min(min(max(fT1, fT2), max(fT3, fT4)), max(fT5, fT6));

	// if tmax < 0, ray is intersecting AABB
	// but entire AABB is behing it's origin
	if (fTMax < 0) {
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (fTmin > fTMax) {
		return false;
	}

	_float fT_Result = fTmin;

	// If tmin is < 0, tmax is closer
	if (fTmin < 0.0f) {
		fT_Result = fTMax;
	}

	if (pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		XMStoreFloat3(&pContact.vContactPoint, XMLoadFloat3(&srcRay.vOrigin) + XMLoadFloat3(&srcRay.vDir) * fT_Result);
		//pContact.vContactNormal = (pContact.vContactPoint - dstBox.Get_Position()).Unit();
		pContact.fPenetration = 0.f;
		pContact.Set_BodyData(srcRay.pBody, dstBox.pBody, pColData->fFriction, pColData->fRestitution);

		_float3 normals[] = {
			_float3(-1, 0, 0),
			_float3(1, 0, 0),
			_float3(0, -1, 0),
			_float3(0, 1, 0),
			_float3(0, 0, -1),
			_float3(0, 0, 1)
		};
		_float t[] = { fT1, fT2, fT3, fT4, fT5, fT6 };

		for (int i = 0; i < 6; ++i) {
			if (CMP((_float)fT_Result, (_float)t[i])) {
				pContact.vContactNormal = normals[i];
			}
		}
	}

	return false;
}

bool FCollisionDetector::RayAndSphere(const FCollisionRay& srcRay, const FCollisionSphere& dstSphere, FCollisionData* pColData)
{
	_vector vDstDir = (dstSphere.Get_PositionVector() - XMLoadFloat3(&srcRay.vOrigin));
	_vector vRayOrigin = XMLoadFloat3(&srcRay.vOrigin);
	_vector vRayDir = XMLoadFloat3(&srcRay.vDir);
	_float fRadiusSq = dstSphere.fRadius * dstSphere.fRadius;

	_float fDstDirSq = XMVectorGetX(XMVector3LengthSq(vDstDir));
	_float fA = XMVectorGetX(XMVector3Dot(vRayDir, vDstDir));
	_float fBSq = fDstDirSq - (fA * fA);
	_float fF = real_sqrt(real_abs(fRadiusSq - fBSq));

	_float fT = fA - fF;

	if (fRadiusSq - fBSq < 0.0f)
	{
		return false;
	}

	if (fDstDirSq < fRadiusSq)
	{
		fT = fA + fF;
	}

	// 충돌정보 생성
	if (pColData != nullptr && pColData->iContactsLeft >= 0)
	{
		FContact& pContact = pColData->tContacts;
		XMStoreFloat3(&pContact.vContactPoint, vRayOrigin + vRayDir * fT);
		XMStoreFloat3(&pContact.vContactNormal, XMVector3Normalize((XMLoadFloat3(&pContact.vContactPoint) - dstSphere.Get_PositionVector())));
		pContact.fPenetration = 0.f;
		pContact.Set_BodyData(srcRay.pBody, dstSphere.pBody, pColData->fFriction, pColData->fRestitution);
	}

	return true;
}

bool FCollisionDetector::RayAndPlane(const FCollisionRay& srcRay, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::RayAndLine(const FCollisionRay& srcRay, const FCollisionLine& dstLine, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::RayAndRay(const FCollisionRay& srcRay, const FCollisionRay& dstRay, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::RayAndTriangle(const FCollisionRay& srcRay, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::RayAndOBB(const FCollisionRay& srcRay, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::TriangleAndPlane(const FCollisionTriangle& srcTriangle, const FCollisionPlane& dstPlane, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::TriangleAndTriangle(const FCollisionTriangle& srcTriangle, const FCollisionTriangle& dstTriangle, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::TriangleAndOBB(const FCollisionTriangle& srcTriangle, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	return false;
}

bool FCollisionDetector::OBBAndOBB(const FCollisionOBB& srcOBB, const FCollisionOBB& dstOBB, FCollisionData* pColData)
{
	return false;
}
