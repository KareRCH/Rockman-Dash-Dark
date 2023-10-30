#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

enum class EUPDATE_TYPE : _uint
{
	UPDATE,
	LATE,
	RENDER,
	SIZE
};
using EUPDATE_T = EUPDATE_TYPE;

enum class EGAMEOBJ_STATE : _uint
{

};
using EGOBJ_ST = EGAMEOBJ_STATE;

END