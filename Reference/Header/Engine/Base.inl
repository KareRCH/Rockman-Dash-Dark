#pragma once

#include "Base.h"

Engine::CBase::CBase()
{
}

_uint Engine::CBase::AddRef()
{
	return ++m_iRefCount;
}

_uint Engine::CBase::Get_RefCount()
{
	return m_iRefCount;
}

_uint Engine::CBase::Release()
{
	if (0U == m_iRefCount)
	{
		Free();
		
		delete this;

		return 0;
	}
	return m_iRefCount--;
}