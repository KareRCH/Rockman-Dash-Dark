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
/// 프로토타입들을 생성하여 관리하는 객체
/// 컴퓨넌터럴
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
	HRESULT						Add_Prototype(const wstring& strProtoKey, CComponent* pPrototype);
	CComponent*					Clone_Prototype(const wstring& strProtoKey, void* pArg);
	void						Clear_Prototypes(const wstring& strContainTag);

private:
	CComponent*					Find_Prototype(const wstring& strProtoKey);

private:
	_unmap<wstring, CComponent*>		m_mapPrototypes;
	using map_proto = _unmap<wstring, CComponent*>;


};

END