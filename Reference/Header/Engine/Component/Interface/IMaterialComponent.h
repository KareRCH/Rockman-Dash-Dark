#pragma once

BEGIN(Engine)

class CMaterialComponent;

class IMaterialComponent abstract
{
public:
	virtual CMaterialComponent* MaterialComp() PURE;
};

END