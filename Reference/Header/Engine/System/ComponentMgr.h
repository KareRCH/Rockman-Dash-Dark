#pragma once

#include "Base.h"


BEGIN(Engine)

class CComponent;

enum class ECompManageType : _uint
{
	System,
	Common,
	Size,
};

/// <summary>
/// ������Ÿ�Ե��� �����Ͽ� �����ϴ� ��ü
/// ��ǻ���ͷ�
/// </summary>
class CComponentMgr final : public CBase
{
	DERIVED_CLASS(CBase, CComponentMgr)

private:
	explicit CComponentMgr() = default;
	explicit CComponentMgr(const CComponentMgr& rhs) = delete;
	virtual ~CComponentMgr() = default;

public:
	HRESULT						Initialize();

public:
	static CComponentMgr*		Create();
private:
	virtual void				Free();

public:
	// ���� ������Ÿ���� �߰��Ѵ�.
	HRESULT						Add_Prototype(const wstring& strTag, const wstring& strProtoKey, CComponent* pPrototype);
	// ������Ÿ���̵� �ν��Ͻ��� ã�� ������ �ν��Ͻ��� ��ȯ�Ѵ�.
	CComponent*					Clone_Prototype(const wstring& strProtoKey, void* pArg);
	CComponent*					Clone_Prototype(const wstring& strProtoKey, class FSerialData& InputData);
	// ������Ÿ���� �����ϴ� ���·� ����� ��� �� �Լ��� �̿��Ѵ�. �ý��� ���ٿ� ������Ʈ�� ����� ��.
	CComponent*					Reference_Prototype(const wstring& strProtoKey);
	// ����
	void						Clear_Prototypes(const wstring& strContainTag);

private:
	// ���������� ���Ǵ� ������Ÿ�� �˻�
	CComponent*					Find_Prototype(const wstring& strProtoKey);

private:
	_unmap<wstring, CComponent*>		m_mapPrototypes;
	using map_proto = _unmap<wstring, CComponent*>;


};

END