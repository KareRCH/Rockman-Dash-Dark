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
	inline virtual _uint Release();

protected:
	inline virtual void Free() PURE;

//public:
//	inline void LockWeak(function<void()> fnRelease)
//	{
//		m_vecFuncWeak.push_back(fnRelease);
//	}
//	void UnlockWeak()
//	{
//		for (size_t i = 0; i < m_vecFuncWeak.size(); i++)
//		{
//			if (m_vecFuncWeak[i])
//				m_vecFuncWeak[i]();
//		}
//		m_vecFuncWeak.clear();
//	}

private:
	_uint m_iRefCount = 0U;
	//vector<function<void()>>	m_vecFuncWeak;
};

/// <summary>
/// CBase 클래스용 위크 포인터
/// </summary>
/// <typeparam name="T"> CBase 클래스 파생 클래스</typeparam>
//template<typename T, typename = enable_if_t<is_base_of<CBase, T>::value>>
//class FBasePtr final
//{
//public:
//	inline FBasePtr() = default;
//	inline ~FBasePtr()
//	{
//		Free();
//	}
//
//public:
//	T* operator = (T* pInstance)
//	{
//		return this->pInstance = pInstance;
//	}
//
//public:
//	inline T* Lock()
//	{
//		if (bLocked)
//			return static_cast<T*>(pInstance);
//
//		static_cast<CBase*>(pInstance)->AddRef();
//		bLocked = true;
//		return static_cast<T*>(pInstance);
//	}
//	inline void Unlock()
//	{
//		if (!bLocked)
//			return;
//
//		Safe_Release(pInstance);
//	}
//	inline _uint Get_RefCount()
//	{
//		if (bLocked)
//			return 0;
//		return static_cast<CBase*>(pInstance)->Get_RefCount();
//	}
//	inline _bool IsLocked()
//	{
//		return bLocked;
//	}
//
//private:
//	/// <summary>
//	/// 소멸자로 자동삭제
//	/// </summary>
//	inline void Free()
//	{
//		Safe_Release(pInstance);
//	}
//
//private:
//	T* pInstance = nullptr;
//	_bool	bLocked = false;
//};

#include "Base.inl"

END


