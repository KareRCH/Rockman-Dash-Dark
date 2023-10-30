#pragma once

#include "Physics/Contact.h"

#include "Physics/RigidBody.h"

BEGIN(Engine)

class ENGINE_DLL FContactGen_Joint : public FContactGenerator
{
	THIS_CLASS(FContactGenerator)

public:
	FRigidBody* pBody[2];
	FVector3	vPosition[2];
	Real		fError;

	void		Set(FRigidBody* pA, const FVector3& A_Pos, FRigidBody* pB, const FVector3& B_Pos, Real fError) {}

	virtual _uint		Add_Contact(FContact* pContact, _uint iLimit) const override { return 0; }
};

END