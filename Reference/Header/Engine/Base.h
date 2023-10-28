#pragma once

#include "Engine_Define.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	inline explicit CBase();
	inline explicit CBase(const CBase& rhs);
	inline virtual ~CBase();

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

END_NAME


