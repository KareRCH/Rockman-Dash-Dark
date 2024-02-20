#pragma once

#include "Client_Define.h"
#include "Character_Common.h"

BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

/// <summary>
/// 핑거리 전용 에너지볼
/// </summary>
class CFingerii_EnergyBall : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CFingerii_EnergyBall)

protected:
	explicit CFingerii_EnergyBall();
	explicit CFingerii_EnergyBall(const CFingerii_EnergyBall& rhs);
	virtual ~CFingerii_EnergyBall() = default;

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
	virtual void OnCreated() override;
	virtual void BeginPlay() override;

public:
	static CFingerii_EnergyBall* Create();
	static CFingerii_EnergyBall* Create(const _float3 vPos);
	CGameObject* Clone(void* Arg = nullptr);

protected:
	virtual void	Free() override;

public:
	virtual FSerialData SerializeData() override;

private:
	HRESULT	Initialize_Component();

public:		// 충돌 이벤트
	virtual void OnCollision(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionEntered(CGameObject* pDst, const FContact* pContact);
	virtual void OnCollisionExited(CGameObject* pDst);

private:
	CCommonModelComp* m_pModelComp = { nullptr };

public:
	void Set_LifeTime(_float Value) { m_fLifeTime = FGauge(Value); }

private:
	FGauge		m_fLifeTime = FGauge(15.f);

public:
	void Set_Target(CCharacter_Common* pTarget);

private:
	CCharacter_Common* m_pTarget = { nullptr };
};

END