#pragma once

#include "Client_Define.h"
#include "GameObject/Character_Common.h"
#include "Utility/LogicDeviceBasic.h"
#include "System/Data/Light.h"

BEGIN(Engine)
class CCommonModelComp;
END

BEGIN(Client)

class CWeapon_BusterCannon : public CCharacter_Common
{
	DERIVED_CLASS(CCharacter_Common, CWeapon_BusterCannon)

protected:
	explicit CWeapon_BusterCannon();
	explicit CWeapon_BusterCannon(const CWeapon_BusterCannon& rhs);
	virtual ~CWeapon_BusterCannon() = default;

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
	virtual void EndPlay() override;

public:
	static CWeapon_BusterCannon* Create();
	static CWeapon_BusterCannon* Create(const _float3 vPos);
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
	GETSET_2(_float, m_fSpeed, Speed, GET_C_REF, SET_C_REF)
		GETSET_2(_float3, m_vLookDir, LookDir, GET_C_REF, SET_C_REF)
		void Set_LifeTime(_float fLifeTime) { m_fLifeTime.Readjust(fLifeTime); }
	void Set_Damage(_float fDamage) { m_fDamage = fDamage; }

private:
	FGauge		m_fLifeTime = FGauge(3.f);
	_float		m_fSpeed = 5.f;
	_float3		m_vLookDir = {};
	_float		m_fDamage = { 1.f };
	CLight* m_pLight = { nullptr };
	_uint		m_iLightID = { 0 };

public:
	void Create_Effect();
};

END