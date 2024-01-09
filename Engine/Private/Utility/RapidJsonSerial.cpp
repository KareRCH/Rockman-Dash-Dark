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

		cout << "JSON ���� ����" << endl;
	}
	else
	{
		cout << "���� ���� ����" << endl;
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
			cout << "JSON ���� �Ľ� ����" << endl;
			return E_FAIL;
		}

		cout << "JSON ���� ����" << endl;
	}
	else
	{
		cout << "���� ���� ����" << endl;
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
	// �߰��Ϸ��� Doc�� ��������� ����
	if (SerialData.m_Doc.HasParseError())
		return E_FAIL;

	// ã�� ����� �ִ��� Ȯ��, Array��� ã��, ������
	const _char* pArrayMember = strArrayMember.c_str();
	Value::MemberIterator iter = m_Doc.FindMember(pArrayMember);
	if (iter == m_Doc.MemberEnd())
	{
		Value Key(pArrayMember, m_Doc.GetAllocator());
		Value NewArray(kArrayType);
		m_Doc.AddMember(Key, NewArray, m_Doc.GetAllocator());
	}

	// Array Value ���� �� �迭 üũ, �迭�� �ٸ� �ø��� ������ ���� �߰�
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
