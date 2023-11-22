#pragma once

#include "Component/D3D11DeviceComp.h"


BEGIN(Engine)

class ID3D11DeviceComp abstract
{
public:
	inline virtual ID3D11Device* const D3D11Device() const PURE;
	inline virtual ID3D11DeviceContext* const D3D11Context() const PURE;
};

END