#pragma once

#include "Component/GameObjectComp.h"

#include "Component/Interface/ITeamAgentComp.h"
#include "Utility/ClassID.h"

BEGIN(Engine)

/// <summary>
/// �Ҽ��� ������ ���� �ʿ��� ������Ʈ
/// �� ������Ʈ�� ������ �Ǹ� ��ȣ�ۿ�� �Ҽ����� ���ΰ��� ���迡 ���� ó���� �� �� �ִ�.
/// Team ����� �̿��ϱ� ���ؼ��� ������ �� ���� ó�� ��ü�� �ʿ��ϴ�.
/// ������Ʈ�� ���� ID�� �������� ��ȣ���� ���� ������ ������.
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
	// ������Ÿ�� ���ۿ� �Լ�
	virtual FSerialData SerializeData_Prototype();
	// Ŭ�δ� ���� �Լ�
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

	// Dst�� ���� ���� ��ȣ���� üũ�Ѵ�.
	static ERelation Check_TeamRelation(CTeamAgentComp* pDst, CTeamAgentComp* pSrc)
	{
		ERelation tRelation = m_mapTeamRelation[pSrc->m_iTeamID][pDst->m_iTeamID];
		if (tRelation == ERelation::None)
			return ERelation::Nuetral;
		return tRelation;
	}

	// Src�� ���赵�� ���� Dst�� ���� ��ȣ���� üũ�Ѵ�.
	static ERelation Check_PrivRelation(CTeamAgentComp* pDst, CTeamAgentComp* pSrc)
	{
		ERelation tRelation = pSrc->m_mapPrivRelation[pDst->m_iPrivID];

		if (tRelation == ERelation::None)
			return ERelation::Nuetral;

		return tRelation;
	}

	// ����, �� ������ ��ȣ���� �����Ѵ�.
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
	_ubyte						m_iTeamID = { 0 };	// �� ID
	_uint_64					m_iPrivID = { 0 };	// ���� ID, �� ������Ʈ ��
	map<_uint_64, ERelation>	m_mapPrivRelation;	// ���� ���� ��



};

using ETeamRelation = CTeamAgentComp::ERelation;

template <>
struct TComponentTrait<CTeamAgentComp::g_ClassID>
{
	using Class = CTeamAgentComp;
};

END