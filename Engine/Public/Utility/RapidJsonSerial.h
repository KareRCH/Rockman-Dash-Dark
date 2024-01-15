#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

using namespace rapidjson;

/// <summary>
/// RapidJson이 사용된 동적 파싱 클래스
/// 기존 버전과 다르게 다양한 데이터 형식을 표현할 수 있도록 제작되었다.
/// </summary>
class ENGINE_DLL FSerialData
{
public:
	FSerialData();
	FSerialData(const FSerialData& rhs);
	~FSerialData() {}

public:
	HRESULT Save_Data(const wstring& strPath);
	HRESULT Load_Data(const wstring& strPath);

public:
	// 멤버에 단순 데이터 추가, String
	HRESULT Add_MemberString(const string& strMemberName, const string& strValue);
	// 멤버에 단순 데이터 추가, Bool
	HRESULT Add_Member(const string& strMemberName, const _bool bValue);
	// 멤버에 단순 데이터 추가, Int
	HRESULT Add_Member(const string& strMemberName, const _int iValue);
	// 멤버에 단순 데이터 추가, Int
	HRESULT Add_Member(const string& strMemberName, const _uint iValue);
	// 멤버에 단순 데이터 추가, Float
	HRESULT Add_Member(const string& strMemberName, const _float fValue);
	// 멤버에 오브젝트 데이터 추가
	HRESULT Add_Member(const string& strMemberName, FSerialData& SerialData);
	// 배열에 다른 Document 추가
	HRESULT Pushback_Member(const string& strArrayMember, FSerialData& SerialData);

public:
	HRESULT Get_Data(const string& strMemberName, _bool& tValue);
	HRESULT Get_Data(const string& strMemberName, string& tValue);
	HRESULT Get_Data(const string& strMemberName, _int& tValue);
	HRESULT Get_Data(const string& strMemberName, _uint& tValue);
	HRESULT Get_Data(const string& strMemberName, _float& tValue);

public:
	_uint Get_ArraySize(const string& strMemberName);
	HRESULT Get_ObjectFromArray(const string& strMemberName, _uint iIndex, FSerialData& DstData);

public:
	Document	m_Doc;

};

END