#ifndef Engine_Typedef_h__
#define Engine_Typedef_h__

#include <stdint.h>
#include <DirectXMath.h>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

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

	// ��Ű��ó�� ���� �Ҽ��� ���е� ���̱�
#ifdef _WIN64
#if _WIN64
	typedef		_double						_real;
#endif // _WIN64 == 1
#else
	typedef		_float						_real;
#endif // _WIN64

	// ����� ����
	typedef		DirectX::XMFLOAT2			_float2;
	typedef		DirectX::XMFLOAT3			_float3;
	typedef		DirectX::XMFLOAT4			_float4;

	// ����� ����
	typedef		DirectX::XMINT2				_int2;
	typedef		DirectX::XMINT3				_int3;
	typedef		DirectX::XMINT4				_int4;

	// ���� ����
	typedef		DirectX::XMVECTOR			_vector;	// �Ϲ����� ��꿡 ���δ�.
	typedef		DirectX::FXMVECTOR			_fvector;	// �Լ� �Ű����� 1 ~ 3
	typedef		DirectX::GXMVECTOR			_gvector;	// 4
	typedef		DirectX::HXMVECTOR			_hvector;	// 5 ~ 6
	typedef		DirectX::CXMVECTOR			_cvector;	// 7

	// ����� ���
	typedef		DirectX::XMFLOAT3X3			_float3x3;
	typedef		DirectX::XMFLOAT3X4			_float3x4;
	typedef		DirectX::XMFLOAT4X3			_float4x3;
	typedef		DirectX::XMFLOAT4X4			_float4x4;

	// ���� ���
	typedef		DirectX::XMMATRIX			_matrix;	// �Ϲ����� ����
	typedef		DirectX::FXMMATRIX			_fmatrix;	// �Լ� ù��° ����
	typedef		DirectX::CXMMATRIX			_cmatrix;	// �Լ� �ι�° ����

	


	// �߰� ���� Ÿ��
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
