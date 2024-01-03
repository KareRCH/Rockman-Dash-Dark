#pragma once

#include "Client_Define.h"
#include "BaseClass/CollisionObject.h"


BEGIN(Engine)

class CCommonModelComp;

END


BEGIN(Client)

/// <summary>
/// 가장 기본이 되는 몬스터 리버봇, 호로꼬
/// </summary>
class CReaverBot_Horokko : public CCollisionObject
{
	DERIVED_CLASS(CCollisionObject, CReaverBot_Horokko)

protected:
	explicit CReaverBot_Horokko();
	explicit CReaverBot_Horokko(const CReaverBot_Horokko& rhs);
	virtual ~CReaverBot_Horokko() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize_Prototype(const _float3 vPos);
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual HRESULT Initialize(const _float3 vPos);
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CReaverBot_Horokko* Create();
	static CReaverBot_Horokko* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

private:
	HRESULT	Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };


public:
	void Move_Update(const _float& fTimeDelta);

public:
	enum class EState_Act { Idle, Run, Ready_Charge, Charge_Attack, Ready_Shooting, Shooting, End_Shooting, Dead };

private:		// 약식 상태머신
	using SState_Act = STATE_SET<EState_Act, void(ThisClass*, const _float&)>;
	SState_Act		m_State_Act;

private:
	void ActState_Idle(const _float& fTimeDelta);
	void ActState_Run(const _float& fTimeDelta);
	void ActState_Ready_Charge(const _float& fTimeDelta);
	void ActState_Charge_Attack(const _float& fTimeDelta);
	void ActState_Ready_Shooting(const _float& fTimeDelta);
	void ActState_Shooting(const _float& fTimeDelta);
	void ActState_End_Shooting(const _float& fTimeDelta);
	void ActState_Dead(const _float& fTimeDelta);


public:
	enum class EState_AI { Idle, Chase, Charge, Charge_Attack, Prowl, Dead };

private:		// 약식 상태머신
	using SState_AI = STATE_SET<EState_AI, void(ThisClass*, const _float&)>;
	SState_AI		m_State_AI;

private:
	void AIState_Idle(const _float& fTimeDelta);
	void AIState_Chase(const _float& fTimeDelta);
	void AIState_Charge(const _float& fTimeDelta);
	void AIState_Charge_Attack(const _float& fTimeDelta);
	void AIState_Prowl(const _float& fTimeDelta);
	void AIState_Dead(const _float& fTimeDelta);

};

END