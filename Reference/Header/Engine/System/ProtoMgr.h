#pragma once

#include "Base.h"


BEGIN(Engine)

class CPrimitiveComponent;


/// <summary>
/// 프로토타입들을 생성하여 관리하는 객체
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
	HRESULT						Create_Proto(const _tchar* pProtoTag, CPrimitiveComponent* pComponent);
	CPrimitiveComponent*		Clone_Proto(const _tchar* pProtoTag);
	CPrimitiveComponent*		Clone_Proto(const _tchar* pProtoTag, CPrimitiveComponent*& prComponent);

private:
	CPrimitiveComponent*		Find_Prototype(const _tchar* pProtoTag);

private:
	_unmap<wstring, CPrimitiveComponent*>		m_mapProto;


};

END