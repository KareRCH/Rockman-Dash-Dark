#pragma once

#include "Base.h"


BEGIN(Engine)

class CComponent;

enum class ECompManageType : _uint
{
	System,
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
	explicit CComponentMgr();
	virtual ~CComponentMgr() = default;

public:
	HRESULT						Initialize();

public:
	static CComponentMgr*		Create();

private:
	virtual void				Free();

public:
	HRESULT						Create_Proto(const wstring& strProtoKey, CComponent* pComponent);
	CComponent*					Clone_Proto(const wstring& strProtoKey);
	CComponent*					Clone_Proto(const wstring& strProtoKey, CComponent*& prComponent);

private:
	CComponent*		Find_Prototype(const wstring& strProtoKey);

private:
	//_unmap<wstring, class CComponent*>	m_pPrototypes[]

private:
	_unmap<wstring, CComponent*>		m_mapPrototypes;
	using map_proto = _unmap<wstring, CComponent*>;


};

END