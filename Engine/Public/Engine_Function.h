#ifndef Engine_Function_h__
#define Engine_Function_h__

#include "Engine_Typedef.h"

using namespace DirectX;

namespace Engine
{
	// ���ø��� ����� ������������ �ڷ����� ���������� ���� ��
	// ����� �ν��Ͻ�ȭ �ϱ� ���Ͽ� �����δ� Ʋ

#pragma region �� �޸�
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
			delete[] Pointer;
			Pointer = nullptr;
		}
	}
#pragma endregion


#pragma region ���۷��� ī��Ʈ
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

	template<typename T>
	_uint Safe_ReleaseAndUnlink(T& pInstance)
	{
		_uint		dwRefCnt = 0;

		if (nullptr != pInstance)
		{
			dwRefCnt = static_cast<_uint>(pInstance->Release());

			if (0 == dwRefCnt)
				pInstance = nullptr;
		}
		pInstance = nullptr;

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
#pragma endregion


#pragma region ���ڿ� �Լ�
	/// <summary>
/// string to wstring ���׷��̵� �Լ�
/// </summary>
/// <param name="str">string</param>
/// <returns></returns>
	wstring Make_Wstring(const string& str)
	{
		return wstring(str.begin(), str.end());
	}
#pragma endregion


#pragma region ���콺
	inline POINT Get_MousePos_Client(const HWND& hWnd)
	{
		POINT mouse;
		::GetCursorPos(&mouse);
		::ScreenToClient(hWnd, &mouse);

		return mouse;
	}

	inline POINT GetClientCenter(const HWND& hWnd) {
		RECT clientRect;
		GetClientRect(hWnd, &clientRect);

		POINT center;
		center.x = clientRect.left + (clientRect.right - clientRect.left) / 2;
		center.y = clientRect.top + (clientRect.bottom - clientRect.top) / 2;

		return center;
	}

	inline void CenterMouse(const HWND& hWnd) {
		POINT center = GetClientCenter(hWnd);
		ClientToScreen(hWnd, &center);
		SetCursorPos(center.x, center.y);
	}
#pragma endregion


#pragma region �����Ϸ��� ĳ���� �Լ�
	// static_cast
	template<typename Return, typename T>
	constexpr Return Cast(T value)
	{
		return static_cast<Return>(value);
	}

	// ���̳��� ĳ��Ʈ
	template<typename Return, typename T>
	constexpr Return DynCast(T value)
	{
		return dynamic_cast<Return>(value);
	}

	// reinterpret_cast
	template<typename Return, typename T>
	constexpr Return ReCast(T value)
	{
		return reinterpret_cast<Return>(value);
	}

	// const_cast
	template<typename Return, typename T>
	constexpr Return ConCast(T value)
	{
		return const_cast<Return>(value);
	}

	// Default Enum Ÿ�� ��ȯ��
	template<typename T, typename = enable_if_t<is_enum<T>::value>,
		typename Return = underlying_type_t<T>>
	constexpr Return ECast(T value)
	{
		return static_cast<Return>(value);
	}

	// void* ��ȯ��
	template<typename T>
	constexpr void* VPCast(T value)
	{
		return static_cast<void*>(value);
	}
#pragma endregion


#pragma region ������ ��� ��� �����
	// ��ķκ��� ��ġ�� ����
	inline _float3 Get_PosFromMatrix(_float4x4 mat)
	{
		return _float3(mat._41, mat._42, mat._43);
	}

	// ��ķκ��� ȸ���� ����
	inline _float3 Get_RotEulerFromMatrix(_float4x4 mat)
	{
		_matrix matRot = {
			mat._11, mat._12, mat._13, 0,
			mat._21, mat._22, mat._23, 0,
			mat._31, mat._32, mat._33, 0,
			0, 0, 0, 1
		};

		_vector vRot = XMQuaternionRotationMatrix(matRot);
		_float3 vEuler = {};
		_float pitch, yaw, roll;
		_float t0, t1;
		t0 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[2]);
		t1 = 1.f - 2.f * (vRot.m128_f32[0] * vRot.m128_f32[0] + vRot.m128_f32[1] * vRot.m128_f32[1]);
		pitch = atan2(t0, t1);

		_float t2;
		t2 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[1] - vRot.m128_f32[2] * vRot.m128_f32[0]);
		t2 = (t2 > 1.f) ? 1.f : t2;
		t2 = (t2 < -1.f) ? -1.f : t2;
		yaw = asin(t2);

		_float t3, t4;
		t3 = 2.f * (vRot.m128_f32[3] * vRot.m128_f32[2] + vRot.m128_f32[0] * vRot.m128_f32[1]);
		t4 = 1.f - 2.f * (vRot.m128_f32[1] * vRot.m128_f32[1] + vRot.m128_f32[2] * vRot.m128_f32[2]);
		roll = atan2(t3, t4);

		return _float3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
	}

	// ��ķκ��� ũ�Ⱚ ����
	inline _float3 Get_ScaleFromMatrix(_float4x4 mat)
	{
		_vector vLength[3] = {
			XMVectorSet(mat._11, mat._12, mat._13, 0.f),
			XMVectorSet(mat._21, mat._22, mat._23, 0.f),
			XMVectorSet(mat._31, mat._32, mat._33, 0.f)
		};

		return _float3(XMVector3Length(vLength[0]).m128_f32[0], XMVector3Length(vLength[1]).m128_f32[1], XMVector3Length(vLength[2]).m128_f32[2]);
	}
#pragma endregion

}

#endif // Engine_Function_h__
