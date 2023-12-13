#pragma once

#include "Engine_Define.h"

BEGIN(Engine)

using namespace rapidjson;

/// <summary>
/// RapidJson이 사용된 동적 파싱 클래스
/// 기존 버전과 다르게 다양한 데이터 형식을 표현할 수 있도록 제작되었다.
/// </summary>
class FSerialData
{
	
public:
	FSerialData() {}
	~FSerialData() {}
public:
	using ValuePair = pair<Type, Value>;			// 타입과 Value를 저장하여 동적으로 데이터를 얻어올 수 있음.
	map<const string, ValuePair>		m_Values;	// 이름으로 데이터 찾기.

public:
	HRESULT Save_Data();
	HRESULT Load_Data();

};

END