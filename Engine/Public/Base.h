#pragma once

#include "Engine_Define.h"

BEGIN_NAME(Engine)

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase();
	explicit CBase(const CBase& rhs);
	virtual ~CBase();

public:
	inline virtual void AddRef();
	inline _uint Get_RefCount();
	inline virtual _uint Release();
	inline virtual void Free();

private:
	_uint m_iRefCount = 0U;
};

END_NAME



CBase::CBase()
{

}

CBase::CBase(const CBase& rhs)
{

}

CBase::~CBase()
{

}

void CBase::AddRef()
{
	++m_iRefCount;
}

_uint CBase::Get_RefCount()
{
	return _uint();
}

_uint CBase::Release()
{
	if (0U == m_iRefCount)
	{
		Free();
	}
	return m_iRefCount--;
}

void CBase::Free()
{
	delete this;
}