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
	unsigned long Safe_Release(T& pInstance)
	{
		unsigned long		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = NULL;
		}

		return dwRefCnt;
	}

	

	// Functor
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const _tchar* pTag) : m_pTargetTag(pTag){}
		~CTag_Finder(void) {}

	public:
		template<typename T> 
		_bool		operator()(const T& pair)
		{
			if (0 == lstrcmpW(m_pTargetTag, pair.first))
				return true;
			
			return false;
		}

	private:
		const _tchar*		m_pTargetTag = nullptr;
	};

	class CDeleteObj
	{
	public:
		explicit CDeleteObj(void) {}
		~CDeleteObj(void) {}
	public: // operator
		template <typename T>
		void operator () (T& pInstance)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = pInstance->Release();

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
	};

	// ���������̳� ������
	class CDeleteMap
	{
	public:
		explicit CDeleteMap(void) {}
		~CDeleteMap(void) {}
	public: // operator	
		template <typename T>
		void operator () (T& Pair)
		{
			_ulong dwRefCnt = 0;

			dwRefCnt = Pair.second->Release();

			if (0 == dwRefCnt)
				Pair.second = NULL;
		}
	};


	inline POINT Get_MousePos_Client(const HWND& hWnd)
	{
		POINT mouse;
		::GetCursorPos(&mouse);
		::ScreenToClient(hWnd, &mouse);

		return mouse;
	}

#pragma region �����Ϸ��� ĳ���� �Լ�
	// �� ������ �ٸ� ��� �������� �����ϴ� ���ø��� �Լ�
	template<typename Return, typename = enable_if_t<is_arithmetic<Return>::value || is_enum<Return>::value>,
		typename T, typename = enable_if_t<is_arithmetic<T>::value || is_enum<T>::value>>
	constexpr Return Cast(T value)
	{
		return static_cast<Return>(value);
	}

	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
	constexpr Return Cast_EnumDef(T value)
	{
		return static_cast<Return>(value);
	}

	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
	Return RCast_EnumDef(T value)
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
