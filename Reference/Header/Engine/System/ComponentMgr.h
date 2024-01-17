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
	// 최초 프로토타입을 추가한다.
	HRESULT						Add_Prototype(const wstring& strTag, const wstring& strProtoKey, CComponent* pPrototype);
	// 프로토타입이될 인스턴스를 찾아 복사한 인스턴스를 반환한다.
	CComponent*					Clone_Prototype(const wstring& strProtoKey, void* pArg);
	CComponent*					Clone_Prototype(const wstring& strProtoKey, class FSerialData& InputData);
	// 프로토타입을 참조하는 형태로 사용할 경우 이 함수를 이용한다. 시스템 접근용 컴포넌트만 사용할 것.
	CComponent*					Reference_Prototype(const wstring& strProtoKey);
	// 정리
	void						Clear_Prototypes(const wstring& strContainTag);

private:
	// 내부적으로 사용되는 프로토타입 검색
	CComponent*					Find_Prototype(const wstring& strProtoKey);

private:
	_unmap<wstring, CComponent*>		m_mapPrototypes;
	using map_proto = _unmap<wstring, CComponent*>;


};

END