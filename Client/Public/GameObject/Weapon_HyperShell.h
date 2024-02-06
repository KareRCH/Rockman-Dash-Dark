#pragma once

#include "Client_Define.h"
#include "Character_Common.h"
#include "Utility/LogicDeviceBasic.h"

BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

class CWeapon_HyperShell : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CWeapon_HyperShell)

protected:
	explicit CWeapon_HyperShell();
	explicit CWeapon_HyperShell(const CWeapon_HyperShell& rhs);
	virtual ~CWeapon_HyperShell() = default;

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
	static CWeapon_HyperShell* Create();
	static CWeapon_HyperShell* Create(const _float3 vPos);
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

private:
	void Create_Effect();

private:
	random_device				m_RandomDevice;
	mt19937_64					m_RandomNumber;

public:
	GETSET_2(_float, m_fSpeed, Speed, GET_C_REF, SET_C_REF)
		void Set_LifeTime(_float fLifeTime) { m_fLifeTime.Readjust(fLifeTime); }

private:
	FGauge		m_fLifeTime = FGauge(5.f);
	_float		m_fSpeed = 1.f;
	FGauge		m_fParticleTime = FGauge(0.1f);
};

END