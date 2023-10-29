#pragma once

#include "Engine_Define.h"

BEGIN_NAME(Engine)

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

END_NAME