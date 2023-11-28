#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// 템플릿은 기능의 정해져있으나 자료형은 정해져있지 않은 것
	// 기능을 인스턴스화 하기 위하여 만들어두는 틀

	template<typename T>
	void	Safe_Delete(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	void	Safe_Delete_Array(T& Pointer)
	{
		if (nullptr != Pointer)
		{
			delete [] Pointer;
			Pointer = nullptr;
		}
	}

	template<typename T>
	_uint Safe_Release(T& pInstance)
	{
		_uint		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = static_cast<_uint>(pInstance->Release());

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}

		return dwRefCnt;
	}

	// 완전해제 함수
	template<typename T>
	_uint Perfect_Release(T& pInstance)
	{
		_uint		dwRefCnt = 0;
		
		while (nullptr != pInstance)
		{
			dwRefCnt = static_cast<_uint>(pInstance->Release());

			if (0 == dwRefCnt)
			{
				pInstance = nullptr;
				break;
			}
			// IUnknown용 컴객체는 1일 때 연결을 끊는다.
			else if (dwRefCnt == 1 && dynamic_cast<IUnknown*>(pInstance))
			{
				pInstance = nullptr;
				break;
			}
		}

		return dwRefCnt;
	}

	template<typename T>
	_uint Safe_AddRef(T& pInstance)
	{
		_uint		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = static_cast<_uint>(pInstance->AddRef());
		}

		return dwRefCnt;
	}

	/// <summary>
	/// string to wstring 업그레이드 함수
	/// </summary>
	/// <param name="str">string</param>
	/// <returns></returns>
	wstring Make_Wstring(const string& str)
	{
		return wstring(str.begin(), str.end());
	}

	inline POINT Get_MousePos_Client(const HWND& hWnd)
	{
		POINT mouse;
		::GetCursorPos(&mouse);
		::ScreenToClient(hWnd, &mouse);

		return mouse;
	}

#pragma region 컴파일러용 캐스팅 함수
	// static_cast버전
	template<typename Return, typename T>
	constexpr Return Cast(T value)
	{
		return static_cast<Return>(value);
	}

	// 다이나믹 캐스트 버전
	template<typename Return, typename T>
	constexpr Return DynCast(T value)
	{
		return dynamic_cast<Return>(value);
	}

	// reinterpret_cast버전
	template<typename Return, typename T>
	constexpr Return ReCast(T value)
	{
		return reinterpret_cast<Return>(value);
	}

	// const_cast버전
	template<typename Return, typename T>
	constexpr Return ConCast(T value)
	{
		return const_cast<Return>(value);
	}

	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
	constexpr Return ECast(T value)
	{
		return static_cast<Return>(value);
	}

	// 아래는 각 타입별로 만든 제네릭 생략 버전
	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _int Cast_Int(T value)
	{
		return static_cast<_int>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _uint Cast_Uint(T value)
	{
		return static_cast<_uint>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _int_8 Cast_Int8(T value)
	{
		return static_cast<_int_8>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _uint_8 Cast_Uint8(T value)
	{
		return static_cast<_uint_8>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _int_16 Cast_Int16(T value)
	{
		return static_cast<_int_16>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _uint_16 Cast_Uint16(T value)
	{
		return static_cast<_uint_16>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _int_64 Cast_Int64(T value)
	{
		return static_cast<_int_64>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _uint_64 Cast_Uint64(T value)
	{
		return static_cast<_uint_64>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _float Cast_Float(T value)
	{
		return static_cast<_float>(value);
	}

	template<typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr _double Cast_Double(T value)
	{
		return static_cast<_double>(value);
	}
#pragma endregion
}

#endif // Engine_Function_h__
