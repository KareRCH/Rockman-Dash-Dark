#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

enum class EComponentID : _uint
{
	Pivot,
	Collider,
	CommonModel,
	BoxModel,
	TerrainModel,
	PlaneModel,
	CylinderModel,
	Navigation,
	CloudStation,
	TeamAgent,
	End = 10000U
};

template <_uint T>
struct TComponentTrait;

enum class EObjectID : _uint
{
	Terrain,
	Navigation,
	End = 10000U
};

template <_uint T>
struct TObjectTrait;

class ENGINE_DLL CClassID abstract final
{
public:
	static _uint g_iCountClassID;
};

END