#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

using namespace rapidjson;

/// <summary>
/// RapidJson�� ���� ���� �Ľ� Ŭ����
/// ���� ������ �ٸ��� �پ��� ������ ������ ǥ���� �� �ֵ��� ���۵Ǿ���.
/// </summary>
class FSerialData
{
	
public:
	FSerialData() {}
	~FSerialData() {}
public:
	using ValuePair = pair<Type, Value>;			// Ÿ�԰� Value�� �����Ͽ� �������� �����͸� ���� �� ����.
	map<const string, ValuePair>		m_Values;	// �̸����� ������ ã��.

public:
	HRESULT Save_Data();
	HRESULT Load_Data();

};

END