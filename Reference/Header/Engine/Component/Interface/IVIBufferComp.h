#pragma once

BEGIN(Engine)

class CVIBufferComp;

class IVIBufferComp abstract
{
public:
	virtual CVIBufferComp* VIBufferComp() PURE;
};

END