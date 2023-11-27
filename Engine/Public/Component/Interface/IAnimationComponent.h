#pragma once

BEGIN(Engine)

class CAnimationComponent;

class IAnimationComponent abstract
{
public:
	virtual CAnimationComponent* AnimComp() PURE;
};

END