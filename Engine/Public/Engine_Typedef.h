#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

#include <stdint.h>

using std::unordered_multimap;
using std::unordered_map;
using std::unordered_set;
using std::pair;

namespace Engine
{
	typedef		bool						_bool;

	typedef		signed char					_byte;
	typedef		unsigned char				_ubyte;

	typedef		char						_char;
	typedef		wchar_t						_tchar;
	
	typedef		signed short				_short;
	typedef		unsigned short				_ushort;

	typedef		signed int					_int;
	typedef		unsigned int				_uint;

	typedef		signed long					_long;
	typedef		unsigned long				_ulong;

	typedef		signed long long			_longlong;
	typedef		unsigned long long			_ulonglong;

	typedef		float						_float;
	typedef		double						_double;

	typedef		int8_t						_int_8;
	typedef		int16_t						_int_16;
	typedef		int32_t						_int_32;
	typedef		int64_t						_int_64;
	typedef		uint8_t						_uint_8;
	typedef		uint16_t					_uint_16;
	typedef		uint32_t					_uint_32;
	typedef		uint64_t					_uint_64;

	// 아키텍처에 따른 소수점 정밀도 높이기
#ifdef _WIN64
#if _WIN64
	typedef		_double						_real;
#endif // _WIN64 == 1
#else
	typedef		_float						_real;
#endif // _WIN64





	// 추가 정의 타입
	template <typename T>
	using _range = pair<T, T>;

	template <typename Key, typename Value>
	using _unmap = unordered_map<Key, Value>;

	template <typename Key, typename Value>
	using _unmulmap = unordered_multimap<Key, Value>;

	template <typename Key>
	using _unset = unordered_set<Key>;
}

#endif // Engine_Typedef_h__
