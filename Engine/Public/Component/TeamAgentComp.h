#pragma once

#include "Component/GameObjectComp.h"

#include "Component/Interface/ITeamAgentComp.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// 소속을 가지기 위해 필요한 컴포넌트
/// 이 컴포넌트를 가지게 되면 상호작용시 소속팀과 서로간의 관계에 의한 처리를 할 수 있다.
/// Team 기능을 이용하기 위해서는 별도의 팀 관계 처리 객체가 필요하다.
/// 컴포넌트는 팀의 ID와 개별적인 우호도에 대한 정보를 가진다.
/// </summary>
class ENGINE_DLL CTeamAgentComp final : public CGameObjectComp
{
	DERIVED_CLASS(CGameObjectComp, CTeamAgentComp)

public:
	static const _uint g_ClassID = ECast(EComponentID::TeamAgent);

protected:
	explicit CTeamAgentComp();
	explicit CTeamAgentComp(const CTeamAgentComp& rhs);
	virtual ~CTeamAgentComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr);
	virtual HRESULT Initialize_Prototype(FSerialData& InputData);
	virtual HRESULT Initialize(void* Arg = nullptr);
	virtual HRESULT Initialize(FSerialData& InputData);
	virtual void	Priority_Tick(const _float& fTimeDelta);
	virtual void	Tick(const _float& fTimeDelta);
	virtual void	Late_Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

public:
	// 프로토타입 제작용 함수
	virtual FSerialData SerializeData_Prototype();
	// 클로닝 전용 함수
	virtual FSerialData SerializeData();

public:
	static	CTeamAgentComp* Create();
	static	CTeamAgentComp* Create(FSerialData& InputData);
	virtual CComponent* Clone(void* Arg = nullptr);
	virtual CComponent* Clone(FSerialData& InputData);

protected:
	virtual void	Free();

public:
	static const _ubyte TEAM_END = (_ubyte)255U;

	enum class ERelation
	{
		None,
		Hostile,
		Nuetral,
		Friend
	};

public:
	void UniqueID() { m_iPrivID = InitID(); }

private:
	static _uint_64 InitID() { return iID_Count++; }

public:
	using SRelationMap = map<_ubyte, ERelation>;

	static void Add_TeamRelation(_uint iSrcTeam, _uint iDstTeam, ERelation eRelation)
	{
		auto iter = m_mapTeamRelation.find(Cast<_ubyte>(iSrcTeam));
		if (iter != m_mapTeamRelation.end())
		{
			m_mapTeamRelation[Cast<_ubyte>(iSrcTeam)].emplace(Cast<_ubyte>(iDstTeam), eRelation);
		}
		else
		{
			SRelationMap mapTeamRelation = SRelationMap();
			mapTeamRelation.emplace(Cast<_ubyte>(iDstTeam), eRelation);
			m_mapTeamRelation.emplace(Cast<_ubyte>(iSrcTeam), mapTeamRelation);
		}
	}

	// Dst에 대한 팀의 우호도를 체크한다.
	static ERelation Check_TeamRelation(CTeamAgentComp* pDst, CTeamAgentComp* pSrc)
	{
		ERelation tRelation = m_mapTeamRelation[pSrc->m_iTeamID][pDst->m_iTeamID];
		if (tRelation == ERelation::None)
			return ERelation::Nuetral;
		return tRelation;
	}

	// Src의 관계도를 통해 Dst에 대한 우호도를 체크한다.
	static ERelation Check_PrivRelation(CTeamAgentComp* pDst, CTeamAgentComp* pSrc)
	{
		ERelation tRelation = pSrc->m_mapPrivRelation[pDst->m_iPrivID];

		if (tRelation == ERelation::None)
			return ERelation::Nuetral;

		return tRelation;
	}

	// 개인, 팀 순으로 우호도를 조사한다.
	static ERelation Check_Relation(CTeamAgentComp* pDst, CTeamAgentComp* pSrc)
	{
		ERelation tRelation = pSrc->m_mapPrivRelation[pDst->m_iPrivID];

		if (tRelation == ERelation::None)
			return Check_TeamRelation(pDst, pSrc);

		return tRelation;
	}

private:
	static _uint_64						iID_Count;
	static map<_ubyte, SRelationMap>	m_mapTeamRelation;

public:
	const _uint Get_TeamID() const { return Cast<_uint>(m_iTeamID); }
	void		Set_TeamID(const _uint iValue) { m_iTeamID = Cast<_ubyte>(iValue); }

private:
	_ubyte						m_iTeamID = { 0 };	// 팀 ID
	_uint_64					m_iPrivID = { 0 };	// 개별 ID, 팀 에이전트 용
	map<_uint_64, ERelation>	m_mapPrivRelation;	// 개별 관계 맵



};

using ETeamRelation = CTeamAgentComp::ERelation;

template <>
struct TComponentTrait<CTeamAgentComp::g_ClassID>
{
	using Class = CTeamAgentComp;
};

END