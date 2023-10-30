#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	inline explicit CBase();
	inline virtual ~CBase() = default;

public:
	inline _uint AddRef();
	inline _uint Get_RefCount();

public:
	inline virtual _uint Release();
	inline virtual void Free() PURE;

private:
	_uint m_iRefCount = 0U;
};

#include "Base.inl"

END


