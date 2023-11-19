#pragma once

#include "Base.h"


BEGIN(Engine)

class CComponent;


/// <summary>
/// ������Ÿ�Ե��� �����Ͽ� �����ϴ� ��ü
/// </summary>
class CProtoMgr final : public CBase
{
	DERIVED_CLASS(CBase, CProtoMgr)

private:
	explicit CProtoMgr();
	virtual ~CProtoMgr() = default;

public:
	HRESULT						Initialize();

public:
	static CProtoMgr*			Create();

private:
	virtual void				Free();

public:
	HRESULT						Create_Proto(const _tchar* pProtoTag, CComponent* pComponent);
	CComponent*					Clone_Proto(const _tchar* pProtoTag);
	CComponent*					Clone_Proto(const _tchar* pProtoTag, CComponent*& prComponent);

private:
	CComponent*		Find_Prototype(const _tchar* pProtoTag);

private:
	_unmap<wstring, CComponent*>		m_mapProto;


};

END