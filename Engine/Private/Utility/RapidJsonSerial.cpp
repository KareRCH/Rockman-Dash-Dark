#include "Utility/RapidJsonSerial.h"

#include "rapidjson/istreamwrapper.h"

FSerialData::FSerialData()
{
	m_Doc.Parse("{}");
}

FSerialData::FSerialData(const FSerialData& rhs)
{
	m_Doc.CopyFrom(rhs.m_Doc, m_Doc.GetAllocator());
}

HRESULT FSerialData::Save_Data(const wstring& strPath)
{
	ofstream ofs(strPath);

	if (ofs.is_open())
	{
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		m_Doc.Accept(writer);

		ofs << buffer.GetString() << endl;

		ofs.close();

		cout << "JSON 저장 성공" << endl;
	}
	else
	{
		cout << "파일 열기 실패" << endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT FSerialData::Load_Data(const wstring& strPath)
{
	ifstream ifs(strPath);

	if (ifs.is_open())
	{
		IStreamWrapper isw(ifs);

		m_Doc.ParseStream(isw);
		
		ifs.close();

		if (m_Doc.HasParseError())
		{
			cout << "JSON 파일 파싱 오류" << endl;
			return E_FAIL;
		}

		cout << "JSON 열기 성공" << endl;
	}
	else
	{
		cout << "파일 열기 실패" << endl;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT FSerialData::Add_MemberString(const string& strMemberName, const string& strValue)
{
	const _char* pMemberName = strMemberName.c_str();
	const _char* pStrValue = strValue.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
	{
		Value KeyValue(pMemberName, m_Doc.GetAllocator());
		Value StringValue(pStrValue, m_Doc.GetAllocator());
		m_Doc.AddMember(KeyValue, StringValue, m_Doc.GetAllocator());

		return S_OK;
	}

	Value& DataValue = iter->value;
	DataValue.SetString(pStrValue, m_Doc.GetAllocator());

	return S_OK;
}

HRESULT FSerialData::Add_Member(const string& strMemberName, const _bool bValue)
{
	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
	{
		Value KeyValue(pMemberName, m_Doc.GetAllocator());
		m_Doc.AddMember(KeyValue, bValue, m_Doc.GetAllocator());

		return S_OK;
	}

	Value& DataValue = iter->value;
	DataValue.SetBool(bValue);

	return S_OK;
}

HRESULT FSerialData::Add_Member(const string& strMemberName, const _int iValue)
{
	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
	{
		Value KeyValue(pMemberName, m_Doc.GetAllocator());
		m_Doc.AddMember(KeyValue, iValue, m_Doc.GetAllocator());

		return S_OK;
	}

	Value& DataValue = iter->value;
	DataValue.SetInt(iValue);

	return S_OK;
}

HRESULT FSerialData::Add_Member(const string& strMemberName, const _uint iValue)
{
	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
	{
		Value KeyValue(pMemberName, m_Doc.GetAllocator());
		m_Doc.AddMember(KeyValue, iValue, m_Doc.GetAllocator());

		return S_OK;
	}

	Value& DataValue = iter->value;
	DataValue.SetUint(iValue);

	return S_OK;
}

HRESULT FSerialData::Add_Member(const string& strMemberName, const _float fValue)
{
	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
	{
		Value KeyValue(pMemberName, m_Doc.GetAllocator());
		m_Doc.AddMember(KeyValue, fValue, m_Doc.GetAllocator());

		return S_OK;
	}

	Value& DataValue = iter->value;
	DataValue.SetFloat(fValue);

	return S_OK;
}

HRESULT FSerialData::Pushback_Member(const string& strArrayMember, FSerialData& SerialData)
{
	// 추가하려는 Doc이 비어있으면 실패
	if (SerialData.m_Doc.HasParseError())
		return E_FAIL;

	// 찾는 멤버가 있는지 확인, Array멤버 찾기, 없을시
	const _char* pArrayMember = strArrayMember.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pArrayMember);
	if (iter == m_Doc.MemberEnd())
	{
		Value Key(pArrayMember, m_Doc.GetAllocator());
		Value NewArray(kArrayType);
		m_Doc.AddMember(Key, NewArray, m_Doc.GetAllocator());
	}

	// Array Value 참조 및 배열 체크, 배열에 다른 시리얼 데이터 정보 추가
	iter = m_Doc.FindMember(pArrayMember);
	Value& arrayValue = iter->value;
	if (arrayValue.IsArray())
	{
		Value CopiedValue(SerialData.m_Doc, m_Doc.GetAllocator());
		arrayValue.PushBack(CopiedValue, m_Doc.GetAllocator());
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FSerialData::Get_Data(const string& strMemberName, _bool& tValue)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsBool())
	{
		tValue = iter->value.GetBool();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FSerialData::Get_Data(const string& strMemberName, string& tValue)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsString())
	{
		tValue = iter->value.GetString();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FSerialData::Get_Data(const string& strMemberName, _int& tValue)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsInt())
	{
		tValue = iter->value.GetInt();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FSerialData::Get_Data(const string& strMemberName, _uint& tValue)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsInt() || ValueData.IsUint())
	{
		tValue = iter->value.GetUint();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FSerialData::Get_Data(const string& strMemberName, _float& tValue)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsFloat())
	{
		tValue = iter->value.GetFloat();
	}
	else
		return E_FAIL;

	return S_OK;
}

_uint FSerialData::Get_ArraySize(const string& strMemberName)
{
	if (m_Doc.HasParseError())
		return 0;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return 0;

	Value& ValueData = iter->value;
	if (ValueData.IsArray())
	{
		return Cast<_uint>(ValueData.GetArray().Size());
	}

	return 0;
}

HRESULT FSerialData::Get_ObjectFromArray(const string& strMemberName, _uint iIndex, FSerialData& DstData)
{
	if (m_Doc.HasParseError())
		return E_FAIL;

	const _char* pMemberName = strMemberName.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pMemberName);
	if (iter == m_Doc.MemberEnd())
		return E_FAIL;

	Value& ValueData = iter->value;
	if (ValueData.IsArray())
	{
		if (iIndex < 0 || iIndex >= Cast<_uint>(ValueData.GetArray().Size()))
			return E_FAIL;

		Value& ObjectData = ValueData.GetArray()[iIndex];
		DstData.m_Doc.CopyFrom(ObjectData, DstData.m_Doc.GetAllocator());
	}
	else
		return E_FAIL;

	return S_OK;
}
