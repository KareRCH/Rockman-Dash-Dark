#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

using namespace rapidjson;

/// <summary>
/// RapidJson�� ���� ���� �Ľ� Ŭ����
/// ���� ������ �ٸ��� �پ��� ������ ������ ǥ���� �� �ֵ��� ���۵Ǿ���.
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
	// ����� �ܼ� ������ �߰�, String
	HRESULT Add_MemberString(const string& strMemberName, const string& strValue);
	// ����� �ܼ� ������ �߰�, Bool
	HRESULT Add_Member(const string& strMemberName, const _bool bValue);
	// ����� �ܼ� ������ �߰�, Int
	HRESULT Add_Member(const string& strMemberName, const _int iValue);
	// ����� �ܼ� ������ �߰�, Int
	HRESULT Add_Member(const string& strMemberName, const _uint iValue);
	// ����� �ܼ� ������ �߰�, Float
	HRESULT Add_Member(const string& strMemberName, const _float fValue);
	// ����� ������Ʈ ������ �߰�
	HRESULT Add_Member(const string& strMemberName, FSerialData& SerialData);
	// �迭�� �ٸ� Document �߰�
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