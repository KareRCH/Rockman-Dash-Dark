#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

namespace Engine
{
	// ���ø��� ����� ������������ �ڷ����� ���������� ���� ��
	// ����� �ν��Ͻ�ȭ �ϱ� ���Ͽ� �����δ� Ʋ

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
			// IUnknown�� �İ�ü�� 1�� �� ������ ���´�.
			else if (dwRefCnt == 1 && dynamic_cast<ID3D11Device*>(pInstance))
			{
				pInstance = nullptr;
			}
		}

		return dwRefCnt;
	}

	// �������� �Լ�
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
			// IUnknown�� �İ�ü�� 1�� �� ������ ���´�.
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

	inline POINT Get_MousePos_Client(const HWND& hWnd)
	{
		POINT mouse;
		::GetCursorPos(&mouse);
		::ScreenToClient(hWnd, &mouse);

		return mouse;
	}

#pragma region �����Ϸ��� ĳ���� �Լ�
	// static_cast����
	template<typename Return, typename T>
	constexpr Return Cast(T value)
	{
		return static_cast<Return>(value);
	}

	// ���̳��� ĳ��Ʈ ����
	template<typename Return, typename T>
	Return DynCast(T value)
	{
		static_assert(is_base_of<Return, T>::value, "Return should be a base class of T");
		return dynamic_cast<Return>(value);
	}

	// reinterpret_cast����
	template<typename Return, typename T>
	constexpr Return ReCast(T value)
	{
		return reinterpret_cast<Return>(value);
	}

	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
	constexpr Return Cast_EnumDef(T value)
	{
		return static_cast<Return>(value);
	}

	// �Ʒ��� �� Ÿ�Ժ��� ���� ���׸� ���� ����
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
