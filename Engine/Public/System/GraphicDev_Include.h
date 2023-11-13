#pragma once

#include "Engine_Typedef.h"
#include "Engine_Macro.h"


BEGIN(Engine)

enum class ERenderTarget_Legacy : _uint
{
	Diffuse,
	Ambient,
	Specular,
	Size
};

/// <summary>
/// PBR¿ë ·»´õ Å¸±ê
/// </summary>
enum class ERenderTarget_PBR : _uint
{
	Albedo,
	Normal,
	Metallic,
	Roughness,
	Size
};

enum class ERenderTarget_Common : _uint
{
	Emission,
	SSAO,
	Size
};


struct FRenderTarget_Desc
{

};

END